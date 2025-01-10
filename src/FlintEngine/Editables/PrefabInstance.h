#pragma once
#include <Core/GameObject.h>

class PrefabInstance
	: public GameObject
{
public:
	PrefabInstance(const std::string& prefab_path);
	PrefabInstance(const PrefabInstance& other);

public:
	const std::string& GetPrefabPath() const;
	const std::optional<std::string>& GetNameOverride() const;
	const std::optional<Vector>& GetSizeOverride() const;
	const std::optional<Vector>& GetPositionOverride() const;

	const std::vector<ObjectComponent*>& GetAdditionalComponents() const;

	static std::unique_ptr<GameObject> ToRuntimeObject(std::unique_ptr<PrefabInstance> editable_object);
	static PrefabInstance* TryCastFromGameObject(GameObject& game_object);

public: // GameObject
	virtual const std::string& GetName() const override;
	virtual void SetName(const std::string& name) override;

	virtual GameObject* GetParent() const override;
	virtual void SetParent(GameObject* parent) override;

	virtual void AddChild(std::unique_ptr<GameObject> child) override;
	virtual void RemoveChild(GameObject& child) override;
	virtual void MoveChild(GameObject* child, IGameObjectContainer& new_container) override;

	virtual void AddComponent(std::unique_ptr<ObjectComponent> component) override;
	virtual void RemoveComponent(size_t component_index) override;
	virtual void ModifyComponentField(std::unique_ptr<ComponentFieldChange> change) override;

	virtual std::unique_ptr<GameObject> Copy() const override;

	virtual GameObjectType GetGameObjectType() const override { return GameObjectType::PrefabInstance; }

public: // ITransformable
	virtual void SetWorldPosition(const Vector& pos) override;
	virtual void SetLocalPosition(const Vector& pos) override;
	// Translate in world space
	virtual void Translate(const Vector& offset) override;

	virtual void SetWorldScale(const Vector& scale) override;
	virtual void SetLocalScale(const Vector& scale) override;

	virtual void SetWorldRotation(float rot) override;
	// Rotate in world space
	virtual void Rotate(float angle) override;
	// Rotates so that the x-axis of the object is heading towards specified world position
	virtual void LookAt(const Vector& pos) override;

private:
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

