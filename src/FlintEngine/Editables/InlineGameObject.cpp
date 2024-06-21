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


const std::string& InlineGameObject::GetName() const
{
	return GameObject::GetName();
}

void InlineGameObject::SetName(const std::string& name)
{
	GameObject::SetName(name);
}

GameObject* InlineGameObject::GetParent() const
{
	return GameObject::GetParent();
}

void InlineGameObject::SetParent(GameObject* parent)
{
	GameObject::SetParent(parent);
}

const std::vector<std::unique_ptr<GameObject>>& InlineGameObject::GetChildren() const
{
	return GameObject::GetChildren();
}

void InlineGameObject::AddChild(std::unique_ptr<GameObject> child)
{
	GameObject::AddChild(std::move(child));
}

void InlineGameObject::RemoveChild(GameObject& child)
{
	GameObject::RemoveChild(child);
}

void InlineGameObject::MoveChild(GameObject* child, IGameObjectContainer& new_container)
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

std::unique_ptr<GameObject> InlineGameObject::Copy() const
{
	InlineGameObject* cpy = new InlineGameObject(*this);
	return std::unique_ptr<GameObject>(cpy);
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