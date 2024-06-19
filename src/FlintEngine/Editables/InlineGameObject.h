#pragma once
#include "IEditableGameObject.h"

class InlineGameObject
    : public IEditableGameObject
{
public:
	InlineGameObject();
	InlineGameObject(const GameObject& other);
	InlineGameObject(const InlineGameObject& other);

	static std::unique_ptr<GameObject> ToRuntimeObject(std::unique_ptr<InlineGameObject> editable_object);

public: // IEditableGameObject
	virtual GameObject& GetResult_Depr() override;
	virtual const GameObject& GetResult_Depr() const override;

	virtual void Serializable_AddChild(std::unique_ptr<IEditableGameObject> child) override;
	virtual void Serializable_RemoveChild(IEditableGameObject& child) override;

	virtual void Serializable_AddComponent(std::unique_ptr<ObjectComponent> component) override;
	virtual void Serializable_RemoveComponent(size_t index) override;
	virtual void Serializable_ModifyComponentField(std::unique_ptr<ComponentFieldChange> change) override;

	virtual std::vector<std::unique_ptr<IGameObject>>& Serializable_GetChildren() override;
	virtual const std::vector<std::unique_ptr<IGameObject>>& Serializable_GetChildren() const override;

	virtual EditableGameObjectType Serializable_GetType() const override;

public: // IGameObject
	virtual const std::string& GetName() const override;
	virtual void SetName(const std::string& name) override;

	virtual IGameObject* GetParent() const override;
	virtual void SetParent(IGameObject* parent) override;

	virtual const std::vector<std::unique_ptr<IGameObject>>& GetChildren() const override;
	virtual void MoveChild(IGameObject* child, IGameObjectContainer& new_container) override;

	virtual void SetEnabled(bool enabled) override;

	virtual void SetScene(Scene* scene, SceneKey) override;

	virtual std::unique_ptr<IGameObject> Copy() const override;

	virtual IUpdateable& GetUpdateable() override;
	virtual const IUpdateable& GetUpdateable() const override;
	virtual ITransformable& GetTransformable() override;
	virtual const ITransformable& GetTransformable() const override;

private:
	std::unique_ptr<GameObject> m_GameObject;
};

