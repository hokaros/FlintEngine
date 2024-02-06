#include "Scene.h"

void Scene::Render()
{
	RenderBackground();

	for (std::unique_ptr<GameObject>& go : m_GameObjects)
	{
		go->RenderUpdate();
	}
}

void Scene::AddGameObject(std::unique_ptr<GameObject> game_object)
{
	m_GameObjects.push_back(std::move(game_object));
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
