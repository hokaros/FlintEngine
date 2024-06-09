#pragma once
#include "IEditableGameObject.h"

class InlineGameObject :
    public IEditableGameObject
{
public:
	InlineGameObject();
	InlineGameObject(const GameObject& other);

	virtual GameObject& GetResult() override;
	virtual const GameObject& GetResult() const override;

	virtual void Serializable_SetName(const std::string& name) override;
	virtual void Serializable_SetSize(const Vector& size) override;
	virtual void Serializable_SetPosition(const Vector& position) override;

	virtual void Serializable_AddChild(std::unique_ptr<IEditableGameObject> child) override;
	virtual void Serializable_RemoveChild(IEditableGameObject& child) override;

	virtual void Serializable_AddComponent(std::unique_ptr<ObjectComponent> component) override;
	virtual void Serializable_RemoveComponent(size_t index) override;
	virtual void Serializable_ModifyComponentField(std::unique_ptr<ComponentFieldChange> change) override;

	virtual std::vector<std::unique_ptr<IEditableGameObject>>& Serializable_GetChildren() override;
	virtual const std::vector<std::unique_ptr<IEditableGameObject>>& Serializable_GetChildren() const override;

	virtual EditableGameObjectType Serializable_GetType() const override;

	static std::unique_ptr<GameObject> ToRuntimeObject(std::unique_ptr<InlineGameObject> editable_object);

private:
	std::unique_ptr<GameObject> m_GameObject;

	std::vector<std::unique_ptr<IEditableGameObject>> m_ChildrenEditables;
};

