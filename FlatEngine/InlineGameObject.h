#pragma once
#include "IEditableGameObject.h"

class InlineGameObject :
    public IEditableGameObject
{
public:
	InlineGameObject(GameObject& game_object);

	virtual GameObject& GetResult() override;

	virtual void Destroy() override;

	virtual void SetName(const std::string& name) override;
	virtual void SetSize(const Vector& size) override;
	virtual void SetPosition(const Vector& position) override;

	virtual void AddChild(std::unique_ptr<IEditableGameObject> child, std::unique_ptr<GameObject> real_child) override;

	virtual void AddComponent(std::unique_ptr<ObjectComponent> component) override;
	virtual void RemoveComponent(size_t index) override;
	virtual void ModifyComponentField(std::unique_ptr<ComponentFieldChange> change) override;

	virtual std::vector<std::unique_ptr<IEditableGameObject>>& GetChildren() override;

private:
	GameObject& m_GameObject;

	std::vector<std::unique_ptr<IEditableGameObject>> m_ChildrenEditables;
};

