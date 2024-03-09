#pragma once
#include "../FlatEngine/imgui/imgui.h"
#include "AssetHandles.h"
#include "SelectedGameObjectManager.h"
#include "AssetExplorer.h"

class HierarchyEditor
{
public:
	void Init(SelectedGameObjectManager& selected_game_object_manager, AssetExplorer& asset_explorer);

	void SetGameObject(std::shared_ptr<EditorGameObjectHandle> handle);

	void Render();

private:
	void RenderObjectHierarchy(std::shared_ptr<EditorGameObjectHandle> node_object, bool is_root);
	void RenderObjectContextMenu(IEditableGameObject& game_object, bool is_root);

	bool GetPrefabPathModal(std::string& path);

	std::unique_ptr<GameObject> CreatePrefabCopy(const std::string& prefab_path);

private:
	std::shared_ptr<EditorGameObjectHandle> m_GameObjectHandle;

	SelectedGameObjectManager* m_SelectedGameObjectManager;
	AssetExplorer* m_AssetExplorer;

	static constexpr size_t s_FilePathSize = 256;
	char m_FilePathBuffer[s_FilePathSize];
};

