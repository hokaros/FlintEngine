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
	static ObjectComponent* DeserializeComponent(const ComponentStringDesc& component_desc, GameObject& owner);

private:
	static void ValidateInputFields(const ComponentStringDesc& component_desc, const ComponentDefinition& component_definition);
};

