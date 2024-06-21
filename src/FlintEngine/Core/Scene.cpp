#include "Scene.h"

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

void Scene::MoveObjectsFrom(Scene&& other_scene)
{
	for (std::unique_ptr<GameObject>& go : other_scene.m_ObjectManager.GetOwnedObjects())
	{
		AddGameObject(std::move(go));
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
