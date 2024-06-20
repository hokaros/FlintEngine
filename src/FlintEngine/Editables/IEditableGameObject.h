#pragma once
#include <Core/GameObject.h>
#include "ComponentFieldDefinition.h"

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

enum class EditableGameObjectType
{
	InlineGameObject,
	PrefabInstance
};

// Interface for GameObject wrappers suited for editing within the Editor
class IEditableGameObject
	: public IGameObject
{
public:
	// TODO: remove
	virtual GameObject& GetResult_Depr() = 0; // Please don't recalculate it on every GetResult() call.
	virtual const GameObject& GetResult_Depr() const = 0;

	virtual void Serializable_ModifyComponentField(std::unique_ptr<ComponentFieldChange> change) = 0;

	virtual EditableGameObjectType Serializable_GetType() const = 0;

	virtual ~IEditableGameObject() = default;
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
	
