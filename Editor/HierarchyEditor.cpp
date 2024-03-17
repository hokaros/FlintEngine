#include "HierarchyEditor.h"

HierarchyEditor::HierarchyEditor()
	: m_PrefabPathPrompt("Prefab path", "Prefab path")
{
}

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
	const char* node_id = runtime_object.GetFileName().empty() == false ? runtime_object.GetFileName().c_str() : "?";
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
		for (const std::unique_ptr<IEditableGameObject>& child : node_object->GetChildren())
		{
			std::shared_ptr<EditorGameObjectHandle> child_handle = std::make_shared<EditorIEditableGameObjectHandle>(child.get());
			RenderObjectHierarchy(child_handle, /*is_root*/ false);
		}

		ImGui::TreePop();
	}
}

void HierarchyEditor::RenderObjectContextMenu(IEditableGameObject& game_object, bool is_root)
{
	if (ImGui::Button("Add child"))
	{
		std::unique_ptr<IEditableGameObject> editor_child = std::make_unique<InlineGameObject>();
		game_object.AddChild(std::move(editor_child));
		ImGui::CloseCurrentPopup();
	}

	if (ImGui::Button("Add prefab child"))
	{
		m_PrefabPathPrompt.Open();
	}
	std::string prefab_path;
	if (m_PrefabPathPrompt.Update(prefab_path))
	{
		std::unique_ptr<PrefabInstance> prefab_instance = std::make_unique<PrefabInstance>(prefab_path);
		if (prefab_instance != nullptr)
		{
			game_object.AddChild(std::move(prefab_instance));
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