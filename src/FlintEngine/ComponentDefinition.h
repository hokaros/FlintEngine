#pragma once
#include <map>
#include <vector>
#include <optional>
#include <type_traits>

#include <Core/ObjectComponent.h>
#include "ComponentFieldDefinition.h"

class ComponentDefinition
{
public:
	using ComponentConstructorT = std::function<std::unique_ptr<ObjectComponent> ()>;

	ComponentDefinition(const std::string& name, ComponentConstructorT constructor);
	ComponentDefinition(const std::string& name, ComponentConstructorT constructor, const std::string& base_comp_name);

	const std::string& GetFileName() const;
	RuntimeTypeCode GetTypeCode() const;
	const ComponentConstructorT& GetConstructor() const;
	std::vector<const ComponentFieldDefinition*> GetFields() const;

	void AddField(const ComponentFieldDefinition& field);

	std::unique_ptr<ObjectComponent> CopyComponent(const ObjectComponent& comp) const;

private:
	std::vector<const ComponentFieldDefinition*> GetBaseComponentFields() const;

	static void Append(std::vector<const ComponentFieldDefinition*>& out_vector, const std::vector<const ComponentFieldDefinition*>& in_vector);

private:
	std::string m_Name;
	RuntimeTypeCode m_TypeCode;
	ComponentConstructorT m_Constructor;
	std::vector<const ComponentFieldDefinition*> m_Fields;

	std::optional<std::string> m_BaseComponentName;
};


class ComponentDefinitionManager
{
public:
	static ComponentDefinitionManager& GetInstance();

	void RegisterComponent(ComponentDefinition& definition);
	void AddComponentField(const std::string& component_name, const ComponentFieldDefinition& field);
	std::vector<const ComponentDefinition*> GetAllComponents() const;

	ComponentDefinition* GetDefinitionFromName(const std::string& name) const;
	ComponentDefinition* GetDefinitionFromTypeCode(RuntimeTypeCode type_code) const;
	ComponentDefinition* GetDefinition(const ObjectComponent& component) const;

private:
	std::map<std::string, ComponentDefinition*> m_ComponentNameToDefinition;
	std::map<RuntimeTypeCode, ComponentDefinition*> m_ComponentTypeCodeToDefinition;
};



#define DECLARE_COMPONENT()								\
private:												\
	static ComponentDefinition s_ComponentDefinition;	\
DECLARE_RTC();


#define DEFINE_COMPONENT(clazz)							\
ComponentDefinition clazz::s_ComponentDefinition =		\
	ComponentDefinition(#clazz, []()					\
		{ return std::make_unique<clazz>(); }			\
);														\
														\
DEFINE_RTC(clazz);

#define DEFINE_COMPONENT_DERIVED(clazz, base_clazz)			\
static_assert(std::is_base_of_v<base_clazz, clazz> == true);\
ComponentDefinition clazz::s_ComponentDefinition =			\
	ComponentDefinition(#clazz, []()						\
		{ return std::make_unique<clazz>(); }				\
, #base_clazz);												\
															\
DEFINE_RTC(clazz);


