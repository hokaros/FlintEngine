#include "EditableScene.h"

std::unique_ptr<Scene> EditableScene::CreateRuntimeObject() const
{
	std::unique_ptr<Scene> runtime_scene = std::make_unique<Scene>();

	runtime_scene->SetBackgroundColor(m_BackgroundColor);
	// TODO: add children

	return runtime_scene;
}

void EditableScene::AddRootObject(std::unique_ptr<IEditableGameObject> game_object)
{
	m_RootObjects.push_back(std::move(game_object));
}

const std::vector<std::unique_ptr<IEditableGameObject>>& EditableScene::GetRootObjects() const
{
	return m_RootObjects;
}

void EditableScene::SetBackgroundColor(const Rgb8& color)
{
	m_BackgroundColor = color;
}

const Rgb8& EditableScene::GetBackgroundColor() const
{
	return m_BackgroundColor;
}
