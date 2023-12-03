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
		const char* field_name = field->GetFieldName().c_str();
		// TODO: szykowanie edytorów fieldów w konstruktorze

		// Na podstawie field->GetValueRTC() znajdŸ odpowiedni edytor fielda
		RuntimeTypeCode field_value_type_code = field->GetValueRTC();
		if (field_value_type_code == SerializableTypeInterface<float>::GetTypeCode())
		{
			float field_value;
			field->GetFieldValue(&m_Component, &field_value);
			ImGui::DragFloat(field_name, &field_value);
			field->SetFieldValue(&m_Component, &field_value);
		}
		else if (field_value_type_code == SerializableTypeInterface<bool>::GetTypeCode())
		{
			bool field_value;
			field->GetFieldValue(&m_Component, &field_value);
			ImGui::Checkbox(field_name, &field_value);
			field->SetFieldValue(&m_Component, &field_value);
		}
		else if (field_value_type_code == SerializableTypeInterface<Vector>::GetTypeCode())
		{
			Vector field_value;
			field->GetFieldValue(&m_Component, &field_value);

			float field_value_f[2];
			field_value_f[0] = field_value.x;
			field_value_f[1] = field_value.y;;
			ImGui::DragFloat2(field_name, field_value_f);

			field_value.x = field_value_f[0];
			field_value.y = field_value_f[1];
			field->SetFieldValue(&m_Component, &field_value);
		}
		else
		{
			std::string field_value = field->GetFieldValue(&m_Component);
			char buffer[64];
			strcpy_s(buffer, field_value.c_str());

			ImGui::SetNextItemWidth(150.0f);
			ImGui::InputText(field_name, buffer, 16);
			field_value = buffer;
			field->SetFieldValue(&m_Component, field_value);
		}
	}
}
