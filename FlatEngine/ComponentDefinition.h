#pragma once
#include <map>
#include <vector>

#include "ObjectComponent.h"
#include "ComponentFieldDefinition.h"

class ComponentDefinition
{
public:
	using ComponentConstructorT = std::function<std::unique_ptr<ObjectComponent> ()>;

	ComponentDefinition(const std::string& name, ComponentConstructorT constructor);

	const std::string& GetName() const;
	RuntimeTypeCode GetTypeCode() const;
	const ComponentConstructorT& GetConstructor() const;
	const std::vector<const ComponentFieldDefinition*> GetFields() const;

	void AddField(const ComponentFieldDefinition& field);

private:
	std::string m_Name;
	RuntimeTypeCode m_TypeCode;
	ComponentConstructorT m_Constructor;
	std::vector<const ComponentFieldDefinition*> m_Fields;
};


class ComponentDefinitionManager
{
public:
	static ComponentDefinitionManager& GetInstance();

	void RegisterComponent(ComponentDefinition& definition);
	void AddComponentField(const std::string& component_name, const ComponentFieldDefinition& field);

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
														\
public:													\
	virtual RuntimeTypeCode GetTypeCode() const override;


// TODO: move to RuntimeTypeCode.h
#define DEFINE_RTC(clazz)											\
RuntimeTypeCode clazz::GetTypeCode() const							\
{																	\
static RuntimeTypeCode s_TypeCode = RuntimeTypeCode::CreateNew();	\
return s_TypeCode;													\
}													


#define DEFINE_COMPONENT(clazz)							\
ComponentDefinition clazz::s_ComponentDefinition =		\
	ComponentDefinition(#clazz, []()					\
		{ return std::make_unique<clazz>(); }			\
);														\
														\
DEFINE_RTC(clazz);


