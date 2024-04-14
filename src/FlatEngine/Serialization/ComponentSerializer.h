#pragma once
#include "ComponentDefinition.h"

#include <map>

struct ComponentStringDesc
{
	std::string type;
	std::map<std::string, std::string> fields;
};

class ComponentSerializer
{
public:
	static std::unique_ptr<ComponentStringDesc> SerializeComponent(const ObjectComponent& component);
	static std::unique_ptr<ObjectComponent> DeserializeComponent(const ComponentStringDesc& component_desc);

private:
	static void ValidateInputFields(const ComponentStringDesc& component_desc, const ComponentDefinition& component_definition);
};

