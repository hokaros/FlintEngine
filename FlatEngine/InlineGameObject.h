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

	virtual void SetName(const std::string& name) override;
	virtual void SetSize(const Vector& size) override;
	virtual void SetPosition(const Vector& position) override;

	virtual void AddChild(std::unique_ptr<IEditableGameObject> child) override;
	virtual void DeleteChild(IEditableGameObject& child) override;

	virtual void AddComponent(std::unique_ptr<ObjectComponent> component) override;
	virtual void RemoveComponent(size_t index) override;
	virtual void ModifyComponentField(std::unique_ptr<ComponentFieldChange> change) override;

	virtual std::vector<std::unique_ptr<IEditableGameObject>>& GetChildren() override;
	virtual const std::vector<std::unique_ptr<IEditableGameObject>>& GetChildren() const override;

	virtual EditableGameObjectType GetType() const override;

	static std::unique_ptr<GameObject> ToRuntimeObject(std::unique_ptr<InlineGameObject> editable_object);

private:
	std::unique_ptr<GameObject> m_GameObject;

	std::vector<std::unique_ptr<IEditableGameObject>> m_ChildrenEditables;
};

