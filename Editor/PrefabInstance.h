#pragma once
#include "IEditableGameObject.h"
class PrefabInstance :
    public IEditableGameObject
{
public:
	virtual GameObject& GetResult() = 0;

	virtual void Destroy() = 0;

	virtual void SetName(const std::string& name) = 0;
	virtual void SetSize(const Vector& size) = 0;
	virtual void SetPosition(const Vector& position) = 0;

	virtual void AddChild(std::unique_ptr<GameObject> child) = 0;

	virtual void AddComponent(std::unique_ptr<ObjectComponent> component) = 0;
	virtual void RemoveComponent(size_t index) = 0;
	virtual void ModifyComponentField(std::unique_ptr<ComponentFieldChange> change) = 0;

private:
	GameObject& m_ResultGameObject;

	std::optional<std::string> m_Name;
	std::optional<Vector> m_Size;
	std::optional<Vector> m_Position;

	std::vector<GameObject*> m_AdditionalChildren;
	std::vector<ObjectComponent*> m_AdditionalComponents;
	std::vector<std::unique_ptr<ComponentFieldChange>> m_ComponentFieldChanges;
};

