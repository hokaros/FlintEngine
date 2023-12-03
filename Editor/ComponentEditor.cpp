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

	for (const ComponentFieldDefinition* field : m_ComponentDefinition.GetFields())
	{
		std::string field_value = field->GetFieldValue(&m_Component);
		char buffer[64];
		strcpy_s(buffer, field_value.c_str());

		ImGui::SetNextItemWidth(150.0f);
		ImGui::InputText(field->GetFieldName().c_str(), buffer, 16);
		field_value = buffer;
		field->SetFieldValue(&m_Component, field_value);
	}
}
