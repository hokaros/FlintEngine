#include "ComponentEditor.h"

ComponentEditor::ComponentEditor(ObjectComponent& component)
	: m_Component(component)
	, m_ComponentDefinition(
		*ComponentDefinitionManager::GetInstance().GetDefinitionFromTypeCode(
			component.GetTypeCode()
		))
{
}

void ComponentEditor::Render()
{
	ImGui::Text("Component: %s", m_ComponentDefinition.GetName().c_str());
}
