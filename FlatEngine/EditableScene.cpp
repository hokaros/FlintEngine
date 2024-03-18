#include "EditableScene.h"

void EditableScene::Render()
{
	for (std::unique_ptr<IEditableGameObject>& root_object : m_RootObjects)
	{
		IEditableGameObject::RenderUpdate(*root_object);
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

void EditableScene::CopyObjectsToScene(Scene& scene) const
{
	for (const std::unique_ptr<IEditableGameObject>& editable_object : m_RootObjects)
	{
		std::unique_ptr<GameObject> runtime_object = std::make_unique<GameObject>(editable_object->GetResult()); // copy

		IEditableGameObject::CopyChildrenToRuntimeObject(*editable_object, *runtime_object);

		scene.AddGameObject(std::move(runtime_object));
	}
}

const std::vector<std::unique_ptr<IEditableGameObject>>& EditableScene::GetSubRootObjects()
{
	return m_RootObjects;
}

void EditableScene::DeleteChild(IEditableGameObject& child)
{
	for (auto it = m_RootObjects.begin(); it != m_RootObjects.end(); it++)
	{
		std::unique_ptr<IEditableGameObject>& c = *it;
		if (c.get() == &child)
		{
			m_RootObjects.erase(it);
			return;
		}
	}
}
