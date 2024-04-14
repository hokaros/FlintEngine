#include "ComponentSerializer.h"

std::unique_ptr<ComponentStringDesc> ComponentSerializer::SerializeComponent(const ObjectComponent& component)
{
	ComponentDefinition* comp_def = ComponentDefinitionManager::GetInstance().GetDefinition(component);

	std::unique_ptr<ComponentStringDesc> comp_serialized = std::make_unique<ComponentStringDesc>();

	comp_serialized->type = comp_def->GetFileName();

	for (const ComponentFieldDefinition* field : comp_def->GetFields())
	{
		comp_serialized->fields.insert({ field->GetFieldName(), field->GetFieldValue(&component) });
	}

	return comp_serialized;
}

std::unique_ptr<ObjectComponent> ComponentSerializer::DeserializeComponent(const ComponentStringDesc& component_desc)
{
	FE_LOG("Loading component: %s", component_desc.type.c_str());

	ComponentDefinition* comp_def = ComponentDefinitionManager::GetInstance().GetDefinitionFromName(component_desc.type);
	if (comp_def == nullptr)
	{
		FE_DATA_ERROR("Invalid component name: %s", component_desc.type.c_str());
		return nullptr;
	}

	std::unique_ptr<ObjectComponent> component = comp_def->GetConstructor() ();

	const std::map<std::string, std::string>& input_fields = component_desc.fields;
	size_t matched_fields = 0;
	for (const ComponentFieldDefinition* field : comp_def->GetFields())
	{
		std::string field_name = field->GetFieldName();
		auto it = input_fields.find(field_name);
		if (it == input_fields.end())
			continue; // This field is not overriden

		const std::string& field_value = it->second;
		field->SetFieldValue(component.get(), field_value);
		matched_fields++;
	}

	if (matched_fields != input_fields.size())
	{
		FE_DATA_ERROR("Some input fields were not matched");
#ifdef _DEBUG
		ValidateInputFields(component_desc, *comp_def);
#endif
	}
	return component;
}

void ComponentSerializer::ValidateInputFields(const ComponentStringDesc& component_desc, const ComponentDefinition& component_definition)
{
	// Find incorrect input fields
	for (auto& input_field : component_desc.fields)
	{
		const std::string& field_name = input_field.first;

		bool found = false;
		for (const ComponentFieldDefinition* field : component_definition.GetFields())
		{
			if (field_name == field->GetFieldName())
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			FE_DATA_ERROR("Unmatched field: %s", field_name.c_str());
		}
	}
}
