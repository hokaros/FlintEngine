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
	void RenderObjectHierarchy(std::shared_ptr<EditorGameObjectHandle> node_object, bool is_root);
	void RenderObjectContextMenu(GameObject& game_object, bool is_root);

private:
	std::shared_ptr<EditorGameObjectHandle> m_GameObjectHandle;

	SelectedGameObjectManager* m_SelectedGameObjectManager;
};

