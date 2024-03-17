#pragma once
#include "../FlatEngine/imgui/imgui.h"
#include "../FlatEngine/PrefabInstance.h"
#include "AssetHandles.h"
#include "SelectedGameObjectManager.h"
#include "AssetExplorer.h"

class ModalStringPrompt
{
public:
	ModalStringPrompt(const char* modal_id, const char* label);

	const char* GetModalId() const;
	bool GetResult(std::string& response);

private:
	static constexpr size_t s_BufferSize = 256;

	char m_Buffer[s_BufferSize];
	std::string m_ModalId;
	std::string m_Label;
};

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

