#include "HierarchyEditor.h"

HierarchyEditor::HierarchyEditor()
	: m_PrefabPathPrompt("Prefab path", "Prefab path")
{
	m_PrefabPathPrompt.SetAcceptCallback("Add", [this](std::string path) {
		AddPrefabChildToModalContext(path);
	});
}

void HierarchyEditor::Init(SelectedObjectManager& selected_game_object_manager, AssetExplorer& asset_explorer)
{
	m_SelectedGameObjectManager = &selected_game_object_manager;
	m_AssetExplorer = &asset_explorer;
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
			RenderObjectHierarchy(m_EditedObjectHandle, nullptr, 0);
		}
	}
	ImGui::End();
}

void HierarchyEditor::RenderObjectHierarchy(std::shared_ptr<EditorUniversalHandle> node_object_handle, IHierarchyEditable* parent, size_t index_in_parent)
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
			std::shared_ptr<EditorGameObjectHandle> child_handle = std::make_shared<EditorIEditableGameObjectHandle>(child.get());
			std::shared_ptr<EditorUniversalHandle> uni_han = std::make_shared<EditorUniversalHandle>(child_handle);
			RenderObjectHierarchy(uni_han, hierarchy_node, i++);
		}

		ImGui::TreePop();
	}
}

void HierarchyEditor::RenderObjectContextMenu(std::shared_ptr<EditorUniversalHandle> object, IHierarchyEditable* parent)
{
	if (std::shared_ptr<EditorGameObjectHandle> go_handle = object->GetGameObjectHandle(); go_handle != nullptr)
	{ // TODO: extract
		IEditableGameObject* game_object = go_handle->GetGameObject();
		// TODO: enable this for IHierarchyEditables
		if (ImGui::Button("Add child"))
		{
			std::unique_ptr<IEditableGameObject> editor_child = std::make_unique<InlineGameObject>();
			game_object->AddChild(std::move(editor_child));
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::Button("Add prefab child"))
		{
			m_PrefabPathPrompt.Open();
			m_ModalContext = game_object;
		}
		m_PrefabPathPrompt.Update();

		ImGui::BeginDisabled(parent == nullptr); // Child nodes only
		{
			if (ImGui::Button("Delete"))
			{
				m_RequestedRemove.emplace(*game_object, *parent);
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

void HierarchyEditor::AddPrefabChildToModalContext(std::string prefab_path)
{
	FE_ASSERT(m_ModalContext != nullptr, "No modal context set");

	std::unique_ptr<PrefabInstance> prefab_instance = std::make_unique<PrefabInstance>(prefab_path);
	if (prefab_instance != nullptr)
	{
		m_ModalContext->AddChild(std::move(prefab_instance));
		ImGui::CloseCurrentPopup();
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
