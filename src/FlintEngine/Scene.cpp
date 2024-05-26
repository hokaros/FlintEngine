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

bool Scene::ShouldRender(GameObject* gameObject)
{
	return true;
}

const ObjectManager& Scene::GetObjectManager() const
{
	return m_ObjectManager;
}
