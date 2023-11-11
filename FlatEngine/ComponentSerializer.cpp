#include "ComponentSerializer.h"

#include "ComponentDefinition.h"

ObjectComponent* ComponentSerializer::DeserializeComponent(const ComponentStringDesc& component_desc, GameObject& owner)
{
	ComponentDefinition* comp_def = ComponentDefinitionManager::GetInstance().GetDefinitionFromName(component_desc.type);
	ObjectComponent* component = comp_def->GetConstructor() (owner);

	const std::map<std::string, std::string>& serialized_fields = component_desc.fields;
	size_t matched_fields = 0;
	for (const ComponentFieldDefinition* field : comp_def->GetFields())
	{
		std::string field_name = field->GetFieldName();
		auto it = serialized_fields.find(field_name);
		if (it == serialized_fields.end())
			continue; // This field is not overriden

		const std::string& field_value = it->second;
		field->SetFieldValue(component, field_value);
		matched_fields++;
	}

	if (matched_fields != serialized_fields.size())
	{
		FE_LOG("Some fields were not found");
	}
	return component;
}
