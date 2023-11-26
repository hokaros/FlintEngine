#include "ComponentDefinition.h"

ComponentDefinition::ComponentDefinition(const std::string& name, ComponentConstructorT constructor)
	: m_Name(name)
	, m_TypeCode(RuntimeTypeCode::INVALID)
	, m_Constructor(constructor)
{
	std::unique_ptr<ObjectComponent> representing_component = constructor();
	m_TypeCode = representing_component->GetTypeCode();

	ComponentDefinitionManager::GetInstance().RegisterComponent(*this);
}

const std::string& ComponentDefinition::GetName() const
{
	return m_Name;
}

RuntimeTypeCode ComponentDefinition::GetTypeCode() const
{
	return m_TypeCode;
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
	m_ComponentTypeCodeToDefinition.insert({ definition.GetTypeCode(), &definition });
}

void ComponentDefinitionManager::AddComponentField(const std::string& component_name, const ComponentFieldDefinition& field)
{
	GetDefinitionFromName(component_name)->AddField(field);
}

std::vector<const ComponentDefinition*> ComponentDefinitionManager::GetAllComponents() const
{
	std::vector<const ComponentDefinition*> components;

	for (auto& entry : m_ComponentTypeCodeToDefinition)
	{
		components.push_back(entry.second);
	}

	return components;
}

ComponentDefinition* ComponentDefinitionManager::GetDefinitionFromName(const std::string& name) const
{
	auto it = m_ComponentNameToDefinition.find(name);
	if (it == m_ComponentNameToDefinition.end())
		return nullptr;

	return it->second;
}

ComponentDefinition* ComponentDefinitionManager::GetDefinitionFromTypeCode(RuntimeTypeCode type_code) const
{
	auto it = m_ComponentTypeCodeToDefinition.find(type_code);
	if (it == m_ComponentTypeCodeToDefinition.end())
		return nullptr;

	return it->second;
}

ComponentDefinition* ComponentDefinitionManager::GetDefinition(const ObjectComponent& component) const
{
	return GetDefinitionFromTypeCode(component.GetTypeCode());
}


