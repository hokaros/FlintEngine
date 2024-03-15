#pragma once
#include "../FlatEngine/GameObject.h"
#include "../FlatEngine/ComponentFieldDefinition.h"

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
{
public:
	virtual GameObject& GetResult() = 0; // Please don't recalculate it on every GetResult() call.
	virtual const GameObject& GetResult() const = 0;

	virtual void Destroy() = 0;

	virtual void SetName(const std::string& name) = 0;
	virtual void SetSize(const Vector& size) = 0;
	virtual void SetPosition(const Vector& position) = 0;

	virtual void AddChild(std::unique_ptr<IEditableGameObject> child) = 0;

	virtual void AddComponent(std::unique_ptr<ObjectComponent> component) = 0;
	virtual void RemoveComponent(size_t index) = 0;
	virtual void ModifyComponentField(std::unique_ptr<ComponentFieldChange> change) = 0;

	virtual std::vector<std::unique_ptr<IEditableGameObject>>& GetChildren() = 0;
	virtual const std::vector<std::unique_ptr<IEditableGameObject>>& GetChildren() const = 0;

	virtual EditableGameObjectType GetType() const = 0;

	virtual ~IEditableGameObject() = default;

	static void CopyChildrenToRuntimeObject(const IEditableGameObject& src, GameObject& dest);
	static void RenderUpdate(IEditableGameObject& editable);
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
	
