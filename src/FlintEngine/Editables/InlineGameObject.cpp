#include "InlineGameObject.h"

InlineGameObject::InlineGameObject()
	: GameObject()
{
}

InlineGameObject::InlineGameObject(const GameObject& other)
	: GameObject(other)
{

}

InlineGameObject::InlineGameObject(const InlineGameObject& other)
	: GameObject(other)
{

}

GameObject& InlineGameObject::GetResult_Depr()
{
	return *this;
}

const GameObject& InlineGameObject::GetResult_Depr() const
{
	return *this;
}

std::unique_ptr<GameObject> InlineGameObject::ToRuntimeObject(std::unique_ptr<InlineGameObject> editable_object)
{
	return std::move(editable_object);
}


const std::string& InlineGameObject::GetName() const
{
	return GameObject::GetName();
}

void InlineGameObject::SetName(const std::string& name)
{
	GameObject::SetName(name);
}

IGameObject* InlineGameObject::GetParent() const
{
	return GameObject::GetParent();
}

void InlineGameObject::SetParent(IGameObject* parent)
{
	GameObject::SetParent(parent);
}

const std::vector<std::unique_ptr<IGameObject>>& InlineGameObject::GetChildren() const
{
	return GameObject::GetChildren();
}

void InlineGameObject::AddChild(std::unique_ptr<IGameObject> child)
{
	GameObject::AddChild(std::move(child));
}

void InlineGameObject::RemoveChild(IGameObject& child)
{
	GameObject::RemoveChild(child);
}

void InlineGameObject::MoveChild(IGameObject* child, IGameObjectContainer& new_container)
{
	GameObject::MoveChild(child, new_container);
}

const std::vector<std::unique_ptr<ObjectComponent>>& InlineGameObject::GetAllComponents() const
{
	return GameObject::GetAllComponents();
}


void InlineGameObject::AddComponent(std::unique_ptr<ObjectComponent> component)
{
	GameObject::AddComponent(std::move(component));
}

void InlineGameObject::RemoveComponent(size_t component_index)
{
	GameObject::RemoveComponent(component_index);
}

void InlineGameObject::ModifyComponentField(std::unique_ptr<ComponentFieldChange> change)
{
	GameObject::ModifyComponentField(std::move(change));
}

void InlineGameObject::SetEnabled(bool enabled)
{
	GameObject::SetEnabled(enabled);
}

void InlineGameObject::SetScene(Scene* scene, SceneKey key)
{
	GameObject::SetScene(scene, key);
}

std::unique_ptr<IGameObject> InlineGameObject::Copy() const
{
	InlineGameObject* cpy = new InlineGameObject(*this);
	return std::unique_ptr<IEditableGameObject>(cpy);
}

IUpdateable& InlineGameObject::GetUpdateable()
{
	return GameObject::GetUpdateable();
}

const IUpdateable& InlineGameObject::GetUpdateable() const
{
	return GameObject::GetUpdateable();
}

ITransformable& InlineGameObject::GetTransformable()
{
	return GameObject::GetTransformable();
}

const ITransformable& InlineGameObject::GetTransformable() const
{
	return GameObject::GetTransformable();
}