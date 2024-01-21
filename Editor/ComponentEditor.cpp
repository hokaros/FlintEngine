#include "ComponentEditor.h"

ComponentEditor::ComponentEditor(ObjectComponent& component, size_t index_in_game_object)
	: m_Component(component)
	, m_ComponentDefinition(
		*ComponentDefinitionManager::GetInstance().GetDefinitionFromTypeCode(
			component.GetTypeCode()
		))
	, m_IndexInGameObject(index_in_game_object)
{
}

void ComponentEditor::Render()
{
	std::string id_str = std::to_string(m_IndexInGameObject);
	ImGui::BeginChild(id_str.c_str(), ImVec2(0, CalculateHeight()));

	ImGui::Text("Component: %s", m_ComponentDefinition.GetName().c_str());
	ImGui::SameLine();
	if (ImGui::Button("-"))
	{
		if (m_Observer != nullptr)
		{
			m_Observer->OnComponentDeleted(m_IndexInGameObject);
		}
	}

	for (const ComponentFieldDefinition* field : m_ComponentDefinition.GetFields())
	{
		RenderField(field);
	}

	ImGui::EndChild();
}

void ComponentEditor::RegisterActionObserver(IComponentActionObserver* observer)
{
	m_Observer = observer;
}

void ComponentEditor::RenderField(const ComponentFieldDefinition* field)
{
	// TODO: szykowanie edytorów fieldów w konstruktorze

	std::stringstream field_label_ss;
	field_label_ss << "##" << m_IndexInGameObject << "." << field->GetFieldName();
	std::string field_label_str = field_label_ss.str();
	const char* field_label = field_label_str.c_str();

	const char* field_name = field->GetFieldName().c_str();

	// Na podstawie field->GetValueRTC() znajdŸ odpowiedni edytor fielda
	RuntimeTypeCode field_value_type_code = field->GetValueRTC();
	if (field_value_type_code == SerializableTypeInterface<float>::GetTypeCode())
	{
		float field_value;
		field->GetFieldValue(&m_Component, &field_value);
		ImGui::DragFloat(field_label, &field_value);
		field->SetFieldValue(&m_Component, &field_value);
	}
	else if (field_value_type_code == SerializableTypeInterface<bool>::GetTypeCode())
	{
		bool field_value;
		field->GetFieldValue(&m_Component, &field_value);
		ImGui::Checkbox(field_label, &field_value);
		field->SetFieldValue(&m_Component, &field_value);
	}
	else if (field_value_type_code == SerializableTypeInterface<Vector>::GetTypeCode())
	{
		Vector field_value;
		field->GetFieldValue(&m_Component, &field_value);

		float field_value_f[2];
		field_value_f[0] = field_value.x;
		field_value_f[1] = field_value.y;;
		ImGui::DragFloat2(field_label, field_value_f);

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
		ImGui::InputText(field_label, buffer, 16);
		field_value = buffer;
		field->SetFieldValue(&m_Component, field_value);
	}

	ImGui::SameLine();
	ImGui::Text(field_name);
}

float ComponentEditor::CalculateHeight() const
{
	const float row_height = ImGui::GetFrameHeightWithSpacing();
	return row_height * (1 + m_ComponentDefinition.GetFields().size());
}
