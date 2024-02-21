#include "HierarchyEditor.h"

void HierarchyEditor::SetGameObject(std::shared_ptr<EditorGameObjectHandle> handle)
{
	m_GameObjectHandle = handle;
}

void HierarchyEditor::Render()
{
	if (ImGui::Begin("Hierarchy Editor"))
	{
		GameObject* go = GetGameObject();
		if (go == nullptr)
		{
			ImGui::Text("No game object selected");
		}
		else
		{
			RenderObjectHierarchy(*go);
		}
	}
	ImGui::End();
}

void HierarchyEditor::RenderObjectHierarchy(GameObject& root_object)
{
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::TreeNode(root_object.GetName().c_str()))
	{
		for (const std::unique_ptr<GameObject>& child : root_object.GetChildren())
		{
			RenderObjectHierarchy(*child);
		}

		ImGui::TreePop();
	}
}

GameObject* HierarchyEditor::GetGameObject() const
{
	if (m_GameObjectHandle == nullptr)
		return nullptr;

	return m_GameObjectHandle->GetGameObject();
}
