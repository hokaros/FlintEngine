#pragma once
#include "AssetHandles.h"
#include "../FlatEngine/imgui/imgui.h"

class HierarchyEditor
{
public:
	void SetGameObject(std::shared_ptr<EditorGameObjectHandle> handle);

	void Render();

private:
	void RenderObjectHierarchy(GameObject& root_object);

	GameObject* GetGameObject() const;

private:
	std::shared_ptr<EditorGameObjectHandle> m_GameObjectHandle;
};

