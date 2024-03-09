#pragma once
#include "IEditableGameObject.h"

class PlainGameObject :
    public IEditableGameObject
{
public:
	PlainGameObject(GameObject& game_object);

	virtual GameObject& GetResult() override;

	virtual void Destroy() override;

	virtual void SetName(const std::string& name) override;
	virtual void SetSize(const Vector& size) override;
	virtual void SetPosition(const Vector& position) override;

	virtual void AddChild(std::unique_ptr<GameObject> child) override;

	virtual void AddComponent(std::unique_ptr<ObjectComponent> component) override;
	virtual void RemoveComponent(size_t index) override;
	virtual void ModifyComponentField(const ComponentFieldChange& change) override;

private:
	GameObject& m_GameObject;
};

