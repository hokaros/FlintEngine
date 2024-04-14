#pragma once
#include <imgui/imgui.h>
#include <PrefabInstance.h>
#include <AssetHandles/EditorUniversalHandle.h>
#include "SelectedObjectManager.h"
#include "Widgets.h"

class HierarchyEditor
{
public:
	HierarchyEditor();

	void Init(SelectedObjectManager& selected_game_object_manager);

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
	void RenderObjectHierarchy(std::shared_ptr<EditorUniversalHandle> node_object_handle, IHierarchyEditable* parent, size_t index_in_parent, bool is_root = false);
	void RenderObjectContextMenu(std::shared_ptr<EditorUniversalHandle> game_object, IHierarchyEditable* parent);

	void ProcessAsyncOperations();

	void AddPrefabChild(IHierarchyEditable& parent, std::string prefab_path);

private:
	std::shared_ptr<EditorUniversalHandle> m_EditedObjectHandle;

	std::optional<RemoveObjectOperation> m_RequestedRemove = std::nullopt;

	SelectedObjectManager* m_SelectedGameObjectManager;

	ModalStringPrompt m_PrefabPathPrompt;
};

