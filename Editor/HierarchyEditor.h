#pragma once
#include "../FlatEngine/imgui/imgui.h"
#include "AssetHandles.h"
#include "SelectedGameObjectManager.h"

class HierarchyEditor
{
public:
	void Init(SelectedGameObjectManager& selected_game_object_manager);

	void SetGameObject(std::shared_ptr<EditorGameObjectHandle> handle);

	void Render();

private:
	void RenderObjectHierarchy(std::shared_ptr<EditorGameObjectHandle> root_object);

private:
	std::shared_ptr<EditorGameObjectHandle> m_GameObjectHandle;

	SelectedGameObjectManager* m_SelectedGameObjectManager;
};

