#include "Scene.h"
#include <Navigation/NavmeshGenerator.h>

Scene::Scene(const Scene& other)
	: m_BackgroundColor(other.m_BackgroundColor)
	, m_Navmesh(other.m_Navmesh)
{
	CopyObjectsFrom(other);
}

void Scene::OnLoaded()
{
	m_ObjectManager.SetAsMain();
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
	RenderBackground(renderer);
	m_Navmesh.Render(renderer);

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

const ObjectManager& Scene::GetObjectManager() const
{
	return m_ObjectManager;
}

void Scene::RegenerateNavmesh()
{
	m_Navmesh.Clear();
	Navigation::NavmeshGenerator::Generate(*this, m_Navmesh);
}

void Scene::SetNavmesh(const Navigation::Navmesh& other)
{
	m_Navmesh = other;
}

const Navigation::Navmesh& Scene::GetNavmesh() const
{
	return m_Navmesh;
}