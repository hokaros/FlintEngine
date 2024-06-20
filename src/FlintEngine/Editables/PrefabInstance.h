#pragma once
#include "InlineGameObject.h"

class PrefabInstance
    : public IEditableGameObject
	, public ITransformable
{
public:
	PrefabInstance(const std::string& prefab_path);

public:
	const std::string& GetPrefabPath() const;
	const std::optional<std::string>& GetNameOverride() const;
	const std::optional<Vector>& GetSizeOverride() const;
	const std::optional<Vector>& GetPositionOverride() const;

	const std::vector<ObjectComponent*>& GetAdditionalComponents() const;

	static std::unique_ptr<GameObject> ToRuntimeObject(std::unique_ptr<PrefabInstance> editable_object);

public: // IEditableGameObject
	virtual GameObject& GetResult_Depr() override;
	virtual const GameObject& GetResult_Depr() const override;

	virtual EditableGameObjectType Serializable_GetType() const override;

public: // IGameObject
	virtual const std::string& GetName() const override;
	virtual void SetName(const std::string& name) override;

	virtual IGameObject* GetParent() const override;
	virtual void SetParent(IGameObject* parent) override;

	virtual const std::vector<std::unique_ptr<IGameObject>>& GetChildren() const override;
	virtual void AddChild(std::unique_ptr<IGameObject> child) override;
	virtual void RemoveChild(IGameObject& child) override;
	virtual void MoveChild(IGameObject* child, IGameObjectContainer& new_container) override;

	virtual const std::vector<std::unique_ptr<ObjectComponent>>& GetAllComponents() const override;
	virtual void AddComponent(std::unique_ptr<ObjectComponent> component) override;
	virtual void RemoveComponent(size_t component_index) override;
	virtual void ModifyComponentField(std::unique_ptr<ComponentFieldChange> change) override;

	virtual void SetEnabled(bool enabled) override;

	virtual void SetScene(Scene* scene, SceneKey) override;

	virtual std::unique_ptr<IGameObject> Copy() const override;

	virtual IUpdateable& GetUpdateable() override;
	virtual const IUpdateable& GetUpdateable() const override;
	virtual ITransformable& GetTransformable() override;
	virtual const ITransformable& GetTransformable() const override;

public: // ITransformable
	// TODO: getters are all direct proxies, can we split const and non-const ITransformable?
	virtual const Vector& GetWorldPosition() const override;
	virtual void SetWorldPosition(const Vector& pos) override;
	virtual Vector GetLocalPosition() const override;
	virtual void SetLocalPosition(const Vector& pos) override;
	// Translate in world space
	virtual void Translate(const Vector& offset) override;

	virtual const Vector& GetWorldScale() const override;
	virtual void SetWorldScale(const Vector& scale) override;
	virtual Vector GetLocalScale() const override;
	virtual void SetLocalScale(const Vector& scale) override;

	virtual float GetWorldRotation() const override;
	virtual void SetWorldRotation(float rot) override;
	virtual float GetLocalRotation() const override;
	// Rotate in world space
	virtual void Rotate(float angle) override;
	// Rotates so that the x-axis of the object is heading towards specified world position
	virtual void LookAt(const Vector& pos) override;

	// Transforms from local space to world space
	virtual Vector TransformPoint(const Vector& local_pos) const override;
	// Transforms from world space to local space
	virtual Vector InvTransformPoint(const Vector& world_pos) const override;

private:
	std::unique_ptr<InlineGameObject> m_ResultGameObject;
	std::string m_PrefabPath;

	std::optional<std::string> m_Name;
	std::optional<Vector> m_Size;
	std::optional<Vector> m_Position;

	std::vector<IEditableGameObject*> m_AdditionalChildren;

	size_t m_OriginalComponentCount;
	std::vector<ObjectComponent*> m_AdditionalComponents;
	std::vector<size_t> m_RemovedComponents;
	std::vector<std::unique_ptr<ComponentFieldChange>> m_ComponentFieldChanges;
};

