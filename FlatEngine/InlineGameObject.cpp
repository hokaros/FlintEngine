#include "InlineGameObject.h"

InlineGameObject::InlineGameObject()
	: m_GameObject(std::make_unique<GameObject>())
{
}

GameObject& InlineGameObject::GetResult()
{
	return *m_GameObject;
}

const GameObject& InlineGameObject::GetResult() const
{
	return *m_GameObject;
}

void InlineGameObject::Destroy()
{
	GameObject::Destroy(m_GameObject.get());
}

void InlineGameObject::SetName(const std::string& name)
{
	m_GameObject->SetName(name);
}

void InlineGameObject::SetSize(const Vector& size)
{
	m_GameObject->SetSize(size);
}

void InlineGameObject::SetPosition(const Vector& position)
{
	m_GameObject->SetPosition(position);
}

void InlineGameObject::AddChild(std::unique_ptr<IEditableGameObject> child)
{
	m_ChildrenEditables.push_back(std::move(child));
}

void InlineGameObject::AddComponent(std::unique_ptr<ObjectComponent> component)
{
	m_GameObject->AddComponent(std::move(component));
}

void InlineGameObject::RemoveComponent(size_t index)
{
	m_GameObject->RemoveComponent(index);
}

void InlineGameObject::ModifyComponentField(std::unique_ptr<ComponentFieldChange> change)
{
	FE_ASSERT(change != nullptr, "No change passed");

	change->field->SetFieldValue(change->component, change->GetValue());
}

std::vector<std::unique_ptr<IEditableGameObject>>& InlineGameObject::GetChildren()
{
	return m_ChildrenEditables;
}

const std::vector<std::unique_ptr<IEditableGameObject>>& InlineGameObject::GetChildren() const
{
	return m_ChildrenEditables;
}

EditableGameObjectType InlineGameObject::GetType() const
{
	return EditableGameObjectType::InlineGameObject;
}

std::unique_ptr<GameObject> InlineGameObject::ToRuntimeObject(std::unique_ptr<InlineGameObject> editable_object)
{
	return std::move(editable_object->m_GameObject);
}
