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

	void SetEditedObject(std::shared_ptr<EditorUniversalHandle> handle);

	void Render();

private:
	class RemoveObjectOperation
	{
	public:
		RemoveObjectOperation(IEditableGameObject& removed_object, IHierarchyEditable& parent);

		void Execute();

	private:
		IEditableGameObject& m_RemovedObject;
		IHierarchyEditable& m_Parent;
	};

private:
	void RenderObjectHierarchy(std::shared_ptr<EditorUniversalHandle> node_object_handle, IHierarchyEditable* parent, size_t index_in_parent);
	void RenderObjectContextMenu(std::shared_ptr<EditorUniversalHandle> game_object, IHierarchyEditable* parent);

	void ProcessAsyncOperations();

private:
	std::shared_ptr<EditorUniversalHandle> m_EditedObjectHandle;

	std::optional<RemoveObjectOperation> m_RequestedRemove = std::nullopt;

	SelectedGameObjectManager* m_SelectedGameObjectManager;
	AssetExplorer* m_AssetExplorer;

	ModalStringPrompt m_PrefabPathPrompt;
};

