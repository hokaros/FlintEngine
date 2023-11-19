#pragma once
#include <iostream>
#include <functional>

#include "ObjectComponent.h"
#include "SerializableTypes.h"

class ComponentFieldDefinition
{
public:
	ComponentFieldDefinition(const std::string& component_name, const std::string& field_name);

	const std::string& GetComponentName() const;
	const std::string& GetFieldName() const;
	void SetFieldValue(ObjectComponent* component, const std::string& value) const;
	std::string GetFieldValue(ObjectComponent* component) const;

private:
	virtual void SetFieldValue(ObjectComponent* component, void* value) const = 0;
	virtual void GetFieldValue(ObjectComponent* component, void* value) const = 0;
	virtual void AllocateValue(void*& value) const = 0;
	virtual void ParseValue(const std::string& str, void* value) const = 0;
	virtual void ValueToString(void* value, std::string& str) const = 0;

private:
	std::string m_ComponentName;
	std::string m_FieldName;
};

template<typename FieldT>
class ComponentFieldDefinitionTyped
	: public ComponentFieldDefinition
{
public:
	using FieldGetterT = std::function<void(const ObjectComponent*, FieldT&)>;
	using FieldSetterT = std::function<void(ObjectComponent*, const FieldT&)>;

	ComponentFieldDefinitionTyped(const std::string& component_name, const std::string& field_name, FieldGetterT getter, FieldSetterT setter);

private:
	virtual void SetFieldValue(ObjectComponent* component, void* value) const override;
	virtual void GetFieldValue(ObjectComponent* component, void* value) const override;
	virtual void AllocateValue(void*& value) const override;
	virtual void ParseValue(const std::string& str, void* value) const override;
	virtual void ValueToString(void* value, std::string& str) const override;

private:
	FieldGetterT m_Getter;
	FieldSetterT m_Setter;
};


#define DECLARE_FIELD(name)															\
private:																			\
	using name##_fieldType = decltype(name);										\
	static ComponentFieldDefinitionTyped<name##_fieldType> name##_fieldDefinition;

#define FIELD_TYPE(component, field) component##::##field##_fieldType

#define DEFINE_FIELD(clazz, field_name)												\
ComponentFieldDefinitionTyped<FIELD_TYPE(clazz, field_name)>						\
clazz::field_name##_fieldDefinition =												\
	ComponentFieldDefinitionTyped<FIELD_TYPE(clazz, field_name)>(					\
		#clazz,																		\
		#field_name,																\
		[](const ObjectComponent* component, FIELD_TYPE(clazz, field_name)& value)	\
		{																			\
			const clazz* component_specific = static_cast<const clazz*>(component);	\
			value = component_specific->field_name;									\
		},																			\
		[](ObjectComponent* component, const FIELD_TYPE(clazz, field_name)& value)	\
		{																			\
			clazz* component_specific = static_cast<clazz*>(component);				\
			component_specific->field_name = value;									\
		}																			\
);



template<typename FieldT>
inline ComponentFieldDefinitionTyped<FieldT>::ComponentFieldDefinitionTyped(const std::string& component_name, const std::string& field_name, FieldGetterT getter, FieldSetterT setter)
	: ComponentFieldDefinition(component_name, field_name)
	, m_Getter(getter)
	, m_Setter(setter)
{
}

template<typename FieldT>
inline void ComponentFieldDefinitionTyped<FieldT>::SetFieldValue(ObjectComponent* component, void* value) const
{
	FieldT* value_typed = reinterpret_cast<FieldT*>(value);
	m_Setter(component, *value_typed);
}

template<typename FieldT>
inline void ComponentFieldDefinitionTyped<FieldT>::GetFieldValue(ObjectComponent* component, void* value) const
{
	FieldT* value_typed = reinterpret_cast<FieldT*>(value);
	m_Getter(component, *value_typed);
}

template<typename FieldT>
inline void ComponentFieldDefinitionTyped<FieldT>::AllocateValue(void*& value) const
{
	value = new FieldT();
}

template<typename FieldT>
inline void ComponentFieldDefinitionTyped<FieldT>::ParseValue(const std::string& str, void* value) const
{
	FieldT* value_typed = reinterpret_cast<FieldT*>(value);
	ParseString<FieldT>(str, *value_typed);
}

template<typename FieldT>
inline void ComponentFieldDefinitionTyped<FieldT>::ValueToString(void* value, std::string& str) const
{
	FieldT* value_typed = reinterpret_cast<FieldT*>(value);
	str = ToString<FieldT>(*value_typed);
}
