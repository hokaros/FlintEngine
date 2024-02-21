#include "HierarchyEditor.h"

void HierarchyEditor::Init(SelectedGameObjectManager& selected_game_object_manager)
{
	m_SelectedGameObjectManager = &selected_game_object_manager;
}

void HierarchyEditor::SetGameObject(std::shared_ptr<EditorGameObjectHandle> handle)
{
	m_GameObjectHandle = std::move(handle);
}

void HierarchyEditor::Render()
{
	if (ImGui::Begin("Hierarchy Editor"))
	{
		if (m_GameObjectHandle == nullptr)
		{
			ImGui::Text("No game object selected");
		}
		else
		{
			RenderObjectHierarchy(m_GameObjectHandle);
		}
	}
	ImGui::End();
}

void HierarchyEditor::RenderObjectHierarchy(std::shared_ptr<EditorGameObjectHandle> root_object_handle)
{
	if(root_object_handle == nullptr)
		return;

	GameObject* root_object = root_object_handle->GetGameObject();
	if (root_object == nullptr)
		return;

	constexpr ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	const bool is_selected = m_SelectedGameObjectManager->IsGameObjectSelected(*root_object);
	ImGuiTreeNodeFlags node_flags = base_flags;
	if (is_selected)
	{
		node_flags |= ImGuiTreeNodeFlags_Selected;
	}

	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	const bool node_open = ImGui::TreeNodeEx(root_object->GetName().c_str(), node_flags);
	if (ImGui::IsItemClicked())
	{
		m_SelectedGameObjectManager->SelectGameObject(root_object_handle);
	}

	if (node_open)
	{
		for (const std::unique_ptr<GameObject>& child : root_object->GetChildren())
		{
			std::shared_ptr<EditorGameObjectHandle> child_handle = std::make_shared<EditorGameObjectHandle>(child.get());
			RenderObjectHierarchy(child_handle);
		}

		ImGui::TreePop();
	}
}
