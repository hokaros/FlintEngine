#pragma once
#include "ObjectComponent.h"

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
};

