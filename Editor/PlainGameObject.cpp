#include "PlainGameObject.h"

PlainGameObject::PlainGameObject(GameObject& game_object)
	: m_GameObject(game_object)
{
}

GameObject& PlainGameObject::GetResult()
{
	return m_GameObject;
}

void PlainGameObject::Destroy()
{
	GameObject::Destroy(&m_GameObject);
}

void PlainGameObject::SetName(const std::string& name)
{
	m_GameObject.SetName(name);
}

void PlainGameObject::SetSize(const Vector& size)
{
	m_GameObject.SetSize(size);
}

void PlainGameObject::SetPosition(const Vector& position)
{
	m_GameObject.SetPosition(position);
}

void PlainGameObject::AddChild(std::unique_ptr<GameObject> child)
{
	m_GameObject.AddChild(std::move(child));
}

void PlainGameObject::AddComponent(std::unique_ptr<ObjectComponent> component)
{
	m_GameObject.AddComponent(std::move(component));
}

void PlainGameObject::RemoveComponent(size_t index)
{
	m_GameObject.RemoveComponent(index);
}

void PlainGameObject::ModifyComponentField(const ComponentFieldChange& change)
{
	ObjectComponent* comp = m_GameObject.GetComponent(change.componentIndex);
	change.field->SetFieldValue(comp, change.GetValue());
}
