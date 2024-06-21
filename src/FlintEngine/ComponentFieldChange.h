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


template<typename ValueT>
// The new value is contained in the change operation
struct ComponentFieldChangeContained
	: public ComponentFieldChange
{
public:
	ComponentFieldChangeContained();

	void SetValue(const ValueT& value);

private:
	ValueT m_Value;
};

template<typename ValueT>
inline void ComponentFieldChangeContained<ValueT>::SetValue(const ValueT& value)
{
	m_Value = value;

	m_ValuePtr = &m_Value;
}

template<typename ValueT>
inline ComponentFieldChangeContained<ValueT>::ComponentFieldChangeContained()
	: ComponentFieldChange(&m_Value)
{}
