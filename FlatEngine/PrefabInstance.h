#pragma once
#include "InlineGameObject.h"

class PrefabInstance :
    public IEditableGameObject
{
public:
	PrefabInstance(const std::string& prefab_path);

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

	static std::unique_ptr<GameObject> ToRuntimeObject(std::unique_ptr<PrefabInstance> editable_object);

	const std::string& GetPrefabPath() const;
	const std::optional<std::string>& GetNameOverride() const;
	const std::optional<Vector>& GetSizeOverride() const;
	const std::optional<Vector>& GetPositionOverride() const;
	
	const std::vector<ObjectComponent*>& GetAdditionalComponents() const;

private:
	std::unique_ptr<InlineGameObject> m_ResultGameObject;
	std::string m_PrefabPath;

	std::optional<std::string> m_Name;
	std::optional<Vector> m_Size;
	std::optional<Vector> m_Position;

	std::vector<GameObject*> m_AdditionalChildren;

	size_t m_OriginalComponentCount;
	std::vector<ObjectComponent*> m_AdditionalComponents;
	std::vector<size_t> m_RemovedComponents;
	std::vector<std::unique_ptr<ComponentFieldChange>> m_ComponentFieldChanges;
};

