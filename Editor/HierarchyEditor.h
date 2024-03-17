#pragma once
#include "../FlatEngine/imgui/imgui.h"
#include "../FlatEngine/PrefabInstance.h"
#include "AssetHandles.h"
#include "SelectedGameObjectManager.h"
#include "AssetExplorer.h"
#include "Widgets.h"

class HierarchyEditor
{
public:
	HierarchyEditor();

	void Init(SelectedGameObjectManager& selected_game_object_manager, AssetExplorer& asset_explorer);

	void SetGameObject(std::shared_ptr<EditorGameObjectHandle> handle);

	void Render();

private:
	void RenderObjectHierarchy(std::shared_ptr<EditorGameObjectHandle> node_object, bool is_root);
	void RenderObjectContextMenu(IEditableGameObject& game_object, bool is_root);

private:
	std::shared_ptr<EditorGameObjectHandle> m_GameObjectHandle;

	SelectedGameObjectManager* m_SelectedGameObjectManager;
	AssetExplorer* m_AssetExplorer;

	ModalStringPrompt m_PrefabPathPrompt;
};

