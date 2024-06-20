#pragma once

class ObjectComponent;
class ComponentFieldDefinition;

struct ComponentFieldChange
{
public:
	ObjectComponent* component;
	const ComponentFieldDefinition* field;

	void* GetValue() const { return m_ValuePtr; }

protected:

	ComponentFieldChange(void* valuePtr)
		: m_ValuePtr(valuePtr)
		, component(nullptr)
		, field(nullptr)
	{}

	void* m_ValuePtr;
};