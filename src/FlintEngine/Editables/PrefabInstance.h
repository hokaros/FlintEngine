#pragma once
#include "InlineGameObject.h"

class PrefabInstance :
    public IEditableGameObject
{
public:
	PrefabInstance(const std::string& prefab_path);

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

