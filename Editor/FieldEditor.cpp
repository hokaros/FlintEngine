#include "FieldEditor.h"

FieldEditor::FieldEditor(ObjectComponent& component, const ComponentFieldDefinition& field)
	: m_Component(component)
	, m_Field(field)
{
	std::stringstream field_label_ss;
	field_label_ss << "##" << field.GetFieldName();
	m_FieldLabel = field_label_ss.str();
}

const ComponentFieldDefinition& FieldEditor::GetField() const
{
	return m_Field;
}

void FieldEditorDefault::Render()
{
	constexpr size_t string_max_size = 64;

	std::string field_value = m_Field.GetFieldValue(&m_Component);
	char buffer[string_max_size];
	strcpy_s(buffer, field_value.c_str());

	ImGui::SetNextItemWidth(150.0f);
	ImGui::InputText(m_FieldLabel.c_str(), buffer, string_max_size);
	field_value = buffer;
	m_Field.SetFieldValue(&m_Component, field_value);
}

void FieldEditorFloat::Render()
{
	float field_value;
	m_Field.GetFieldValue(&m_Component, &field_value);
	ImGui::DragFloat(m_FieldLabel.c_str(), &field_value);
	m_Field.SetFieldValue(&m_Component, &field_value);
}

void FieldEditorBool::Render()
{
	bool field_value;
	m_Field.GetFieldValue(&m_Component, &field_value);
	ImGui::Checkbox(m_FieldLabel.c_str(), &field_value);
	m_Field.SetFieldValue(&m_Component, &field_value);
}

void FieldEditorVector::Render()
{
	Vector field_value;
	m_Field.GetFieldValue(&m_Component, &field_value);

	float field_value_f[2];
	field_value_f[0] = field_value.x;
	field_value_f[1] = field_value.y;;
	ImGui::DragFloat2(m_FieldLabel.c_str(), field_value_f);

	field_value.x = field_value_f[0];
	field_value.y = field_value_f[1];
	m_Field.SetFieldValue(&m_Component, &field_value);
}

void FieldEditorRgb8::Render()
{
	Rgb8 field_value;
	m_Field.GetFieldValue(&m_Component, &field_value);

	float field_value_f[3];
	field_value.ToFloat(field_value_f[0], field_value_f[1], field_value_f[2]);
	ImGui::ColorEdit3(m_FieldLabel.c_str(), field_value_f);

	field_value = Rgb8::FromFloat(field_value_f[0], field_value_f[1], field_value_f[2]);
	m_Field.SetFieldValue(&m_Component, &field_value);
}

void FieldEditorDouble::Render()
{
	double field_value;
	m_Field.GetFieldValue(&m_Component, &field_value);

	float field_float = field_value;
	ImGui::DragFloat(m_FieldLabel.c_str(), &field_float);
	field_value = field_float;

	m_Field.SetFieldValue(&m_Component, &field_value);
}
