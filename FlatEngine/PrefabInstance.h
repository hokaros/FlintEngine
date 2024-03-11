#pragma once
#include "InlineGameObject.h"

class PrefabInstance :
    public IEditableGameObject
{
public:
	PrefabInstance(GameObject& game_object, const GameObject* prefab);

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
	InlineGameObject m_ResultGameObject;
	const GameObject* m_Prefab;

	std::optional<std::string> m_Name;
	std::optional<Vector> m_Size;
	std::optional<Vector> m_Position;

	std::vector<GameObject*> m_AdditionalChildren;

	size_t m_OriginalComponentCount;
	std::vector<ObjectComponent*> m_AdditionalComponents;
	std::vector<size_t> m_RemovedComponents;
	std::vector<std::unique_ptr<ComponentFieldChange>> m_ComponentFieldChanges;
};

