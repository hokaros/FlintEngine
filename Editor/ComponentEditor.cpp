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

	const char* field_name = field->GetFieldName().c_str();
	std::unique_ptr<FieldEditor> field_editor = CreateFieldEditor(*field);
	field_editor->Render();

	ImGui::SameLine();
	ImGui::Text(field_name);
}

std::unique_ptr<FieldEditor> ComponentEditor::CreateFieldEditor(const ComponentFieldDefinition& field)
{
	RuntimeTypeCode field_value_type_code = field.GetValueRTC();
	if (field_value_type_code == SerializableTypeInterface<float>::GetTypeCode())
	{
		return std::make_unique<FieldEditorFloat>(m_Component, field);
	}
	else if (field_value_type_code == SerializableTypeInterface<bool>::GetTypeCode())
	{
		return std::make_unique<FieldEditorBool>(m_Component, field);
	}
	else if (field_value_type_code == SerializableTypeInterface<Vector>::GetTypeCode())
	{
		return std::make_unique<FieldEditorVector>(m_Component, field);
	}
	else
	{
		return std::make_unique<FieldEditorDefault>(m_Component, field);
	}
}

float ComponentEditor::CalculateHeight() const
{
	const float row_height = ImGui::GetFrameHeightWithSpacing();
	return row_height * (1 + m_ComponentDefinition.GetFields().size());
}
