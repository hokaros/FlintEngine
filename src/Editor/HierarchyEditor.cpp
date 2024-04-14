#include "HierarchyEditor.h"

#include <AssetManager.h>

HierarchyEditor::HierarchyEditor()
	: m_PrefabPathPrompt("Prefab path", "Prefab path")
{
}

void HierarchyEditor::Init(SelectedObjectManager& selected_game_object_manager)
{
	m_SelectedGameObjectManager = &selected_game_object_manager;
}

void HierarchyEditor::SetEditedObject(std::shared_ptr<EditorUniversalHandle> handle)
{
	FE_ASSERT(handle == nullptr || handle->GetHierarchyEditable() != nullptr, "No HierarchyEditable passed");

	m_EditedObjectHandle = std::move(handle);
}

void HierarchyEditor::Render()
{
	ProcessAsyncOperations();

	if (ImGui::Begin("Hierarchy Editor"))
	{
		if (m_EditedObjectHandle == nullptr)
		{
			ImGui::Text("Nothing to show");
		}
		else
		{
			RenderObjectHierarchy(m_EditedObjectHandle, /*parent*/nullptr, /*index_in_parent*/0, /*is_root*/true);
		}

		m_PrefabPathPrompt.Update();
	}
	ImGui::End();
}

void HierarchyEditor::RenderObjectHierarchy(std::shared_ptr<EditorUniversalHandle> node_object_handle, IHierarchyEditable* parent, size_t index_in_parent, bool is_root)
{
	if (node_object_handle == nullptr)
		return;

	IHierarchyEditable* hierarchy_node = node_object_handle->GetHierarchyEditable();
	if (hierarchy_node == nullptr)
		return;

	constexpr ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	const bool is_selected = m_SelectedGameObjectManager->IsObjectSelected(*node_object_handle);
	ImGuiTreeNodeFlags node_flags = base_flags;
	if (is_selected)
	{
		node_flags |= ImGuiTreeNodeFlags_Selected;
	}

	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	ImGui::PushID(index_in_parent);

	const bool node_open = ImGui::TreeNodeEx(hierarchy_node->GetName(), node_flags);
	if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
	{
		m_SelectedGameObjectManager->SelectObject(node_object_handle);
	}
	if (is_root && node_object_handle->HasUnsavedChanges())
	{
		ImGui::SameLine();
		ImGui::Text("*");
	}

	if (ImGui::BeginPopupContextItem("Tree node context menu"))
	{
		RenderObjectContextMenu(node_object_handle, parent);
		ImGui::EndPopup();
	}
	ImGui::PopID();

	if (node_open)
	{
		size_t i = 0;
		for (const std::unique_ptr<IEditableGameObject>& child : hierarchy_node->GetSubRootObjects())
		{
			std::shared_ptr<EditorGameObjectHandle> child_handle = std::make_shared<EditorIEditableGameObjectHandle>(*child, *m_EditedObjectHandle);
			std::shared_ptr<EditorUniversalHandle> uni_han = std::make_shared<EditorUniversalHandle>(child_handle);
			RenderObjectHierarchy(uni_han, hierarchy_node, i++);
		}

		ImGui::TreePop();
	}
}

void HierarchyEditor::RenderObjectContextMenu(std::shared_ptr<EditorUniversalHandle> object, IHierarchyEditable* parent)
{
	if (IHierarchyEditable* edited_node = object->GetHierarchyEditable())
	{
		if (ImGui::Button("Add child"))
		{
			ImGui::CloseCurrentPopup();

			std::unique_ptr<IEditableGameObject> editor_child = std::make_unique<InlineGameObject>();
			edited_node->AddChild(std::move(editor_child));
		}

		if (ImGui::Button("Add prefab child"))
		{
			ImGui::CloseCurrentPopup();

			m_PrefabPathPrompt.SetAcceptCallback("Add", [this, edited_node](std::string prefab_path) {
				AddPrefabChild(*edited_node, prefab_path);
			});
			m_PrefabPathPrompt.Open();
		}
	}

	if (EditorGameObjectHandle* game_object = object->GetGameObjectHandle())
	{
		ImGui::BeginDisabled(parent == nullptr); // Child nodes only
		{
			if (ImGui::Button("Delete"))
			{
				m_RequestedRemove.emplace(game_object->GetGameObject(), *parent);
				m_SelectedGameObjectManager->OnObjectDestroying(*object);
			}
		}
		ImGui::EndDisabled();
	}
}

void HierarchyEditor::ProcessAsyncOperations()
{
	if (m_RequestedRemove.has_value())
	{
		RemoveObjectOperation& op = m_RequestedRemove.value();
		op.Execute();

		m_RequestedRemove.reset();
	}
}

void HierarchyEditor::AddPrefabChild(IHierarchyEditable& parent, std::string prefab_path)
{
	if (AssetManager::GetInstance()->GetPrefab(prefab_path) != nullptr)
	{
		std::unique_ptr<PrefabInstance> prefab_instance = std::make_unique<PrefabInstance>(prefab_path);
		parent.AddChild(std::move(prefab_instance));
	}
	else
	{
		// TODO: visual error
		FE_LOG("No such prefab");
	}
}

HierarchyEditor::RemoveObjectOperation::RemoveObjectOperation(IEditableGameObject& removed_object, IHierarchyEditable& parent)
	: m_RemovedObject(removed_object)
	, m_Parent(parent)
{
}

void HierarchyEditor::RemoveObjectOperation::Execute()
{
	m_Parent.DeleteChild(m_RemovedObject);
}
