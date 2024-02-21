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
			RenderObjectHierarchy(m_GameObjectHandle, /*is_root*/ true);
		}
	}
	ImGui::End();
}

void HierarchyEditor::RenderObjectHierarchy(std::shared_ptr<EditorGameObjectHandle> node_object_handle, bool is_root)
{
	if(node_object_handle == nullptr)
		return;

	GameObject* node_object = node_object_handle->GetGameObject();
	if (node_object == nullptr)
		return;

	constexpr ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	const bool is_selected = m_SelectedGameObjectManager->IsGameObjectSelected(*node_object);
	ImGuiTreeNodeFlags node_flags = base_flags;
	if (is_selected)
	{
		node_flags |= ImGuiTreeNodeFlags_Selected;
	}

	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	const bool node_open = ImGui::TreeNodeEx(node_object->GetName().c_str(), node_flags);
	if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
	{
		m_SelectedGameObjectManager->SelectGameObject(node_object_handle);
	}

	if (ImGui::BeginPopupContextItem("Tree node context menu"))
	{
		RenderObjectContextMenu(*node_object, is_root);
		ImGui::EndPopup();
	}

	if (node_open)
	{
		for (const std::unique_ptr<GameObject>& child : node_object->GetChildren())
		{
			std::shared_ptr<EditorGameObjectHandle> child_handle = std::make_shared<EditorGameObjectHandle>(child.get());
			RenderObjectHierarchy(child_handle, /*is_root*/ false);
		}

		ImGui::TreePop();
	}
}

void HierarchyEditor::RenderObjectContextMenu(GameObject& game_object, bool is_root)
{
	if (ImGui::Button("Add child"))
	{
		game_object.AddChild(std::make_unique<GameObject>(PrefabCreationKey()));
		ImGui::CloseCurrentPopup();
	}

	ImGui::BeginDisabled(is_root); // Child nodes only
	{
		if (ImGui::Button("Delete"))
		{
			GameObject::Destroy(&game_object);
		}
	}
	ImGui::EndDisabled();
}
