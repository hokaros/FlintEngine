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


