#include "Scene.h"

void Scene::PreFrame()
{
	m_ObjectManager.ActivateNewObjects();
}

void Scene::Update()
{
	for (GameObject* go : m_ObjectManager.GetAllObjects()) {
		go->Update();
	}
}

void Scene::Render()
{
	RenderBackground();

	for (GameObject* go : m_ObjectManager.GetAllObjects()) 
	{
		if (go->renderUnseen) 
		{
			go->RenderUpdate();
			continue;
		}

		// TODO: we can refactor vision a lot
		bool canSee = ShouldRender(go);
		if (canSee)
		{
			go->RenderUpdate();
		}
	}
}

void Scene::PostFrame()
{
	m_ObjectManager.DisposeDestroyed();
}

void Scene::AddGameObject(std::unique_ptr<GameObject> game_object)
{
	m_ObjectManager.AddObject(std::move(game_object));
}

void Scene::SetBackgroundColor(const Rgb8& color)
{
	m_BackgroundColor = color;
}

void Scene::RenderBackground()
{
	SceneRenderer* renderer = SceneRenderer::Main();
	renderer->RenderRect(renderer->GetRenderedRect(), m_BackgroundColor);
}

bool Scene::ShouldRender(GameObject* gameObject)
{
	return true;
}
