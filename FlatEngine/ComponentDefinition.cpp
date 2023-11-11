#include "ComponentDefinition.h"

ComponentDefinition::ComponentDefinition(const std::string& name, ComponentConstructorT constructor)
	: m_Name(name)
	, m_Constructor(constructor)
{
	ComponentDefinitionManager::GetInstance().RegisterComponent(*this);
}

const std::string& ComponentDefinition::GetName() const
{
	return m_Name;
}

const ComponentDefinition::ComponentConstructorT& ComponentDefinition::GetConstructor() const
{
	return m_Constructor;
}

const std::vector<const ComponentFieldDefinition*> ComponentDefinition::GetFields() const
{
	return m_Fields;
}

void ComponentDefinition::AddField(const ComponentFieldDefinition& field)
{
	m_Fields.push_back(&field);
}

ComponentDefinitionManager& ComponentDefinitionManager::GetInstance()
{
	static ComponentDefinitionManager s_Instance;
	return s_Instance;
}



void ComponentDefinitionManager::RegisterComponent(ComponentDefinition& definition)
{
	m_ComponentNameToDefinition.insert({ definition.GetName(), &definition });
}

void ComponentDefinitionManager::AddComponentField(const std::string& component_name, const ComponentFieldDefinition& field)
{
	GetDefinitionFromName(component_name)->AddField(field);
}

ComponentDefinition* ComponentDefinitionManager::GetDefinitionFromName(const std::string& name) const
{
	auto it = m_ComponentNameToDefinition.find(name);
	if (it == m_ComponentNameToDefinition.end())
		return nullptr;

	return it->second;
}


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

std::string ComponentFieldDefinition::GetFieldValue(ObjectComponent* component) const
{
	void* value_unstringified;
	AllocateValue(value_unstringified);
	GetFieldValue(component, value_unstringified);

	std::string stringified_value;
	ValueToString(value_unstringified, stringified_value);
	return stringified_value;
}
