#pragma once
#include "IEditableGameObject.h"

class InlineGameObject
	: public GameObject
{
public:
	InlineGameObject();
	InlineGameObject(const GameObject& other);
	InlineGameObject(const InlineGameObject& other);

	static std::unique_ptr<GameObject> ToRuntimeObject(std::unique_ptr<InlineGameObject> editable_object);

public: // GameObject
	virtual const std::string& GetName() const override;
	virtual void SetName(const std::string& name) override;

	virtual GameObject* GetParent() const override;
	virtual void SetParent(GameObject* parent) override;

	virtual const std::vector<std::unique_ptr<GameObject>>& GetChildren() const override;
	virtual void AddChild(std::unique_ptr<GameObject> child) override;
	virtual void RemoveChild(GameObject& child) override;
	virtual void MoveChild(GameObject* child, IGameObjectContainer& new_container) override;

	virtual const std::vector<std::unique_ptr<ObjectComponent>>& GetAllComponents() const override;
	virtual void AddComponent(std::unique_ptr<ObjectComponent> component) override;
	virtual void RemoveComponent(size_t component_index) override;
	virtual void ModifyComponentField(std::unique_ptr<ComponentFieldChange> change) override;

	virtual void SetEnabled(bool enabled) override;

	virtual void SetScene(Scene* scene, SceneKey) override;

	virtual std::unique_ptr<GameObject> Copy() const override;

	virtual IUpdateable& GetUpdateable() override;
	virtual const IUpdateable& GetUpdateable() const override;
	virtual ITransformable& GetTransformable() override;
	virtual const ITransformable& GetTransformable() const override;

	virtual GameObjectType GetGameObjectType() const { return GameObjectType::GameObject; }
};

