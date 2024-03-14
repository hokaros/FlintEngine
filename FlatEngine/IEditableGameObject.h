#pragma once
#include "../FlatEngine/GameObject.h"
#include "../FlatEngine/ComponentFieldDefinition.h"

struct ComponentFieldChange
{
public:
	ObjectComponent* component;
	const ComponentFieldDefinition* field;

	void* GetValue() const { return valuePtr; }

protected:
	ComponentFieldChange() = default;

	void* valuePtr;
};

template<typename ValueT>
// The new value is contained in the change operation
struct ComponentFieldChangeContained
	: public ComponentFieldChange
{
public:
	void SetValue(const ValueT& value);

private:
	ValueT value;
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
};

template<typename ValueT>
inline void ComponentFieldChangeContained<ValueT>::SetValue(const ValueT& value)
{
	this->value = value;

	this->valuePtr = &(this->value);
}
