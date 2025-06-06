#include "Scene.h"
#include <Navigation/NavmeshGenerator.h>
#include <Dbg/DebugConfig.h>
#include <Scene/Camera.h>

Scene::Scene(const Scene& other)
	: m_BackgroundColor(other.m_BackgroundColor)
	, m_Navmesh(other.m_Navmesh)
	, m_NavmeshGenerationParams(other.m_NavmeshGenerationParams)
{
	CopyObjectsFrom(other);
}

void Scene::OnLoaded()
{
	m_ObjectManager.SetAsMain();
	RegenerateNavmesh();
}

void Scene::Update()
{
	m_ObjectManager.ActivateNewObjects();

	for (GameObject* go : m_ObjectManager.GetAllMessageSubscribers()) 
	{
		go->GetUpdateable().Update();
	}
}

void Scene::Render(SceneRenderer& renderer)
{
	SetViewportBasedOnCamera(renderer);
	RenderBackground(renderer);

	if (debug::DebugConfig::ShouldDebugRenderNavmesh())
	{
		m_Navmesh.Render(renderer);
	}

	for (GameObject* go : m_ObjectManager.GetAllMessageSubscribers()) 
	{
		go->GetUpdateable().RenderUpdate(renderer);
	}
}

void Scene::ResetScene()
{
	m_ObjectManager.DestroyAll();
	m_ObjectManager.Clear();
}

void Scene::PostFrame()
{
	m_ObjectManager.DisposeDestroyed();
}

GameObject* Scene::FindGameObjectByName(const std::string& name) const
{
	for (const std::unique_ptr<GameObject>& go : m_ObjectManager.GetOwnedObjects())
	{
		if (go->GetName() == name)
		{
			return go.get();
		}
	}

	return nullptr;
}

Scene::GameObjectsT::iterator Scene::BeginRootGameObjects()
{
	return m_ObjectManager.GetOwnedObjects().begin();
}

Scene::GameObjectsT::iterator Scene::EndRootGameObjects()
{
	return m_ObjectManager.GetOwnedObjects().end();
}

void Scene::AddGameObject(std::unique_ptr<GameObject> game_object)
{
	game_object->SetScene(this, {});

	m_ObjectManager.AddNewObject(std::move(game_object));
}

const std::vector<std::unique_ptr<GameObject>>& Scene::GetGameObjects() const
{
	return m_ObjectManager.GetOwnedObjects();
}

void Scene::RemoveGameObject(GameObject& game_object)
{
	m_ObjectManager.RemoveGameObject(game_object);
}

void Scene::MoveObjectsFrom(Scene&& other_scene)
{
	for (std::unique_ptr<GameObject>& go : other_scene.m_ObjectManager.GetOwnedObjects())
	{
		AddGameObject(std::move(go));
	}
}

void Scene::CopyObjectsFrom(const Scene& other)
{
	for (const std::unique_ptr<GameObject>& object : other.m_ObjectManager.GetOwnedObjects())
	{
		AddGameObject(object->Copy());
	}
}

void Scene::SetBackgroundColor(const Rgb8& color)
{
	m_BackgroundColor = color;
}

const Rgb8& Scene::GetBackgroundColor() const
{
	return m_BackgroundColor;
}

void Scene::RenderBackground(SceneRenderer& renderer)
{
	renderer.Clear(m_BackgroundColor);
}

void Scene::SetViewportBasedOnCamera(SceneRenderer& renderer)
{
	if (const Camera* current_camera = m_CameraManager.GetCurrentCamera())
	{
		renderer.SetViewport(current_camera->CalculateViewport());
	}
}

const ObjectManager& Scene::GetObjectManager() const
{
	return m_ObjectManager;
}

scene::CameraManager& Scene::GetCameraManager()
{
	return m_CameraManager;
}

void Scene::RegenerateNavmesh()
{
	m_Navmesh.Clear();
	Navigation::NavmeshGenerator::Generate(*this, m_NavmeshGenerationParams, m_Navmesh);
}

void Scene::SetNavmesh(const Navigation::Navmesh& other)
{
	m_Navmesh = other;
}

const Navigation::Navmesh& Scene::GetNavmesh() const
{
	return m_Navmesh;
}

const Navigation::NavmeshGenerationParams& Scene::GetNavmeshGenerationParams() const
{
	return m_NavmeshGenerationParams;
}

Navigation::NavmeshGenerationParams& Scene::GetNavmeshGenerationParams()
{
	return m_NavmeshGenerationParams;
}

void Scene::SetNavmeshGenerationParams(const Navigation::NavmeshGenerationParams& params)
{
	m_NavmeshGenerationParams = params;
}