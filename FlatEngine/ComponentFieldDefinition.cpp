#include "ComponentFieldDefinition.h"
#include "ComponentDefinition.h"

ComponentFieldDefinition::ComponentFieldDefinition(const std::string& component_name, const std::string& field_name)
	: m_ComponentName(component_name)
	, m_FieldName(field_name)
{
	ComponentDefinitionManager::GetInstance().AddComponentField(component_name, *this);
}

const std::string& ComponentFieldDefinition::GetComponentName() const
{
	return m_ComponentName;
}

const std::string& ComponentFieldDefinition::GetFieldName() const
{
	return m_FieldName;
}

void ComponentFieldDefinition::SetFieldValue(ObjectComponent* component, const std::string& value) const
{
	void* parsed_value;
	AllocateValue(parsed_value);
	ParseValue(value, parsed_value);
	SetFieldValue(component, parsed_value);
}

std::string ComponentFieldDefinition::GetFieldValue(const ObjectComponent* component) const
{
	void* value_unstringified;
	AllocateValue(value_unstringified);
	GetFieldValue(component, value_unstringified);

	std::string stringified_value;
	ValueToString(value_unstringified, stringified_value);
	return stringified_value;
}
