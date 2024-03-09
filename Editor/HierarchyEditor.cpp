#include "HierarchyEditor.h"

void HierarchyEditor::Init(SelectedGameObjectManager& selected_game_object_manager, AssetExplorer& asset_explorer)
{
	m_SelectedGameObjectManager = &selected_game_object_manager;
	m_AssetExplorer = &asset_explorer;
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

	IEditableGameObject* node_object = node_object_handle->GetGameObject();
	if (node_object == nullptr)
		return;

	constexpr ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	GameObject& runtime_object = node_object->GetResult();
	const bool is_selected = m_SelectedGameObjectManager->IsGameObjectSelected(runtime_object);
	ImGuiTreeNodeFlags node_flags = base_flags;
	if (is_selected)
	{
		node_flags |= ImGuiTreeNodeFlags_Selected;
	}

	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	const char* node_id = runtime_object.GetName().empty() == false ? runtime_object.GetName().c_str() : "?";
	const bool node_open = ImGui::TreeNodeEx(node_id, node_flags);
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
		for (const std::unique_ptr<GameObject>& child : runtime_object.GetChildren())
		{
			std::shared_ptr<EditorGameObjectHandle> child_handle = std::make_shared<EditorPlainGameObjectHandle>(child.get());
			RenderObjectHierarchy(child_handle, /*is_root*/ false);
		}

		ImGui::TreePop();
	}
}

void HierarchyEditor::RenderObjectContextMenu(IEditableGameObject& game_object, bool is_root)
{
	if (ImGui::Button("Add child"))
	{
		game_object.AddChild(std::make_unique<GameObject>(PrefabCreationKey()));
		ImGui::CloseCurrentPopup();
	}

	if (ImGui::Button("Add prefab child"))
	{
		ImGui::OpenPopup("Prefab path");
	}
	std::string prefab_path;
	if (GetPrefabPathModal(prefab_path))
	{
		std::unique_ptr<GameObject> prefab_copy = CreatePrefabCopy(prefab_path);
		if (prefab_copy != nullptr)
		{
			game_object.AddChild(std::move(prefab_copy));
			ImGui::CloseCurrentPopup();
		}
	}

	ImGui::BeginDisabled(is_root); // Child nodes only
	{
		if (ImGui::Button("Delete"))
		{
			game_object.Destroy();
		}
	}
	ImGui::EndDisabled();
}

bool HierarchyEditor::GetPrefabPathModal(std::string& path)
{
	bool has_accepted = false;

	if (ImGui::BeginPopupModal("Prefab path", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::InputText("Prefab path", m_FilePathBuffer, s_FilePathSize);

		if (ImGui::Button("Add"))
		{
			path = m_FilePathBuffer;
			has_accepted = true;

			ImGui::CloseCurrentPopup();
		}

		if (ImGui::Button("Cancel")) 
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	return has_accepted;
}

std::unique_ptr<GameObject> HierarchyEditor::CreatePrefabCopy(const std::string& prefab_path)
{
	GameObject* prefab = m_AssetExplorer->GetPrefab(prefab_path);
	if (prefab == nullptr)
		return nullptr;

	return std::make_unique<GameObject>(*prefab, PrefabCreationKey());
}
