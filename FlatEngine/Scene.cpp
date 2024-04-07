#include "Scene.h"

void Scene::Update()
{
	m_ObjectManager.ActivateNewObjects();

	for (GameObject* go : m_ObjectManager.GetAllMessageSubscribers()) 
	{
		go->Update();
	}
}

void Scene::Render(SceneRenderer& renderer)
{
	RenderBackground(renderer);

	for (GameObject* go : m_ObjectManager.GetAllMessageSubscribers()) 
	{
		if (go->renderUnseen) 
		{
			go->RenderUpdate(renderer);
			continue;
		}

		// TODO: we can refactor vision a lot
		const bool canSee = ShouldRender(go);
		if (canSee)
		{
			go->RenderUpdate(renderer);
		}
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

void Scene::AddGameObject(std::unique_ptr<GameObject> game_object)
{
	m_ObjectManager.AddNewObject(std::move(game_object));
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

bool Scene::ShouldRender(GameObject* gameObject)
{
	return true;
}

ObjectManager& Scene::GetObjectManager()
{
	return m_ObjectManager;
}

const ObjectManager& Scene::GetObjectManager() const
{
	return m_ObjectManager;
}
