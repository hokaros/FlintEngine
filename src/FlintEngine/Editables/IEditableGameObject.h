#pragma once
#include <Core/GameObject.h>
#include "ComponentFieldDefinition.h"

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
	
