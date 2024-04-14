#include "ComponentDefinition.h"

ComponentDefinition::ComponentDefinition(const std::string& name, ComponentConstructorT constructor)
	: m_Name(name)
	, m_TypeCode(RuntimeTypeCode::INVALID)
	, m_Constructor(constructor)
	, m_BaseComponentName(std::nullopt)
{
	std::unique_ptr<ObjectComponent> representing_component = constructor();
	m_TypeCode = representing_component->GetTypeCode();

	ComponentDefinitionManager::GetInstance().RegisterComponent(*this);
}

ComponentDefinition::ComponentDefinition(const std::string& name, ComponentConstructorT constructor, const std::string& base_comp_name)
	: ComponentDefinition(name, constructor)
{
	m_BaseComponentName = base_comp_name;
}

const std::string& ComponentDefinition::GetFileName() const
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

std::vector<const ComponentFieldDefinition*> ComponentDefinition::GetFields() const
{
	std::vector<const ComponentFieldDefinition*> fields = GetBaseComponentFields();
	Append(fields, m_Fields);

	return fields;
}

void ComponentDefinition::AddField(const ComponentFieldDefinition& field)
{
	m_Fields.push_back(&field);
}

std::unique_ptr<ObjectComponent> ComponentDefinition::CopyComponent(const ObjectComponent& comp) const
{
	std::unique_ptr<ObjectComponent> copy = m_Constructor();
	
	// Copy fields
	for (const ComponentFieldDefinition* field : GetFields())
	{
		std::string value = field->GetFieldValue(&comp);
		field->SetFieldValue(copy.get(), value);
	}

	return copy;
}

std::vector<const ComponentFieldDefinition*> ComponentDefinition::GetBaseComponentFields() const
{
	if(m_BaseComponentName.has_value() == false)
		return std::vector<const ComponentFieldDefinition*>(); // No base component

	ComponentDefinition* base_component = ComponentDefinitionManager::GetInstance().GetDefinitionFromName(m_BaseComponentName.value());
	FE_ASSERT(base_component != nullptr, "%s - Cannot find base component: %s", GetFileName(), m_BaseComponentName.value());

	return base_component->GetFields();
}

void ComponentDefinition::Append(std::vector<const ComponentFieldDefinition*>& out_vector, const std::vector<const ComponentFieldDefinition*>& in_vector)
{
	for (const ComponentFieldDefinition* c : in_vector)
	{
		out_vector.push_back(c);
	}
}

ComponentDefinitionManager& ComponentDefinitionManager::GetInstance()
{
	static ComponentDefinitionManager s_Instance;
	return s_Instance;
}



void ComponentDefinitionManager::RegisterComponent(ComponentDefinition& definition)
{
	m_ComponentNameToDefinition.insert({ definition.GetFileName(), &definition });
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