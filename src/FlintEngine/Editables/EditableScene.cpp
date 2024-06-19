#include "EditableScene.h"

void EditableScene::Render(SceneRenderer& renderer)
{
	m_ResultScene.Render(renderer);

	for (std::unique_ptr<IGameObject>& root_object : m_RootObjects)
	{
		root_object->GetUpdateable().RenderUpdate(renderer);
	}
}

std::unique_ptr<Scene> EditableScene::CreateRuntimeObject() const
{
	std::unique_ptr<Scene> runtime_scene = std::make_unique<Scene>();

	runtime_scene->SetBackgroundColor(m_BackgroundColor);

	CopyObjectsToScene(*runtime_scene);

	return runtime_scene;
}

void EditableScene::AddRootObject(std::unique_ptr<IEditableGameObject> game_object)
{
	m_RootObjects.push_back(std::move(game_object));
}

void EditableScene::RemoveRootObject(IGameObject& game_object)
{
	for (auto it = m_RootObjects.begin(); it != m_RootObjects.end(); it++)
	{
		std::unique_ptr<IGameObject>& c = *it;
		if (c.get() == &game_object)
		{
			m_RootObjects.erase(it);
			return;
		}
	}
}

const std::vector<std::unique_ptr<IGameObject>>& EditableScene::GetRootObjects() const
{
	return m_RootObjects;
}

void EditableScene::SetBackgroundColor(const Rgb8& color)
{
	m_BackgroundColor = color;
	m_ResultScene.SetBackgroundColor(color);
}

const Rgb8& EditableScene::GetBackgroundColor() const
{
	return m_BackgroundColor;
}

void EditableScene::CopyObjectsToScene(Scene& scene) const
{
	for (const std::unique_ptr<IGameObject>& object : m_RootObjects)
	{
		std::unique_ptr<IGameObject> runtime_object = object->Copy();

		scene.AddGameObject(std::move(runtime_object));
	}
}