#include "InlineGameObject.h"

InlineGameObject::InlineGameObject()
	: m_GameObject(std::make_unique<GameObject>())
{
}

InlineGameObject::InlineGameObject(const GameObject& other)
	: m_GameObject(std::make_unique<GameObject>(other))
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

void InlineGameObject::Serializable_AddChild(std::unique_ptr<IEditableGameObject> child)
{
	m_ChildrenEditables.push_back(std::move(child));
}

void InlineGameObject::Serializable_RemoveChild(IEditableGameObject& child)
{
	for (auto it = m_ChildrenEditables.begin(); it != m_ChildrenEditables.end(); it++)
	{
		std::unique_ptr<IEditableGameObject>& c = *it;
		if (c.get() == &child)
		{
			m_ChildrenEditables.erase(it);
			return;
		}
	}
}

void InlineGameObject::Serializable_AddComponent(std::unique_ptr<ObjectComponent> component)
{
	m_GameObject->AddComponent(std::move(component));
}

void InlineGameObject::Serializable_RemoveComponent(size_t index)
{
	m_GameObject->RemoveComponent(index);
}

void InlineGameObject::Serializable_ModifyComponentField(std::unique_ptr<ComponentFieldChange> change)
{
	FE_ASSERT(change != nullptr, "No change passed");

	change->field->SetFieldValue(change->component, change->GetValue());
}

std::vector<std::unique_ptr<IEditableGameObject>>& InlineGameObject::Serializable_GetChildren()
{
	return m_ChildrenEditables;
}

const std::vector<std::unique_ptr<IEditableGameObject>>& InlineGameObject::Serializable_GetChildren() const
{
	return m_ChildrenEditables;
}

EditableGameObjectType InlineGameObject::Serializable_GetType() const
{
	return EditableGameObjectType::InlineGameObject;
}

std::unique_ptr<GameObject> InlineGameObject::ToRuntimeObject(std::unique_ptr<InlineGameObject> editable_object)
{
	if (editable_object == nullptr)
		return nullptr;

	CopyChildrenToRuntimeObject(*editable_object, editable_object->GetResult());
	return std::move(editable_object->m_GameObject);
}


const std::string& InlineGameObject::GetName() const
{
	return m_GameObject->GetName();
}

void InlineGameObject::SetName(const std::string& name)
{
	m_GameObject->SetName(name);
}

IGameObject* InlineGameObject::GetParent() const
{
	return m_GameObject->GetParent();
}

void InlineGameObject::SetParent(IGameObject* parent)
{
	m_GameObject->SetParent(parent);
}

const std::vector<std::unique_ptr<IGameObject>>& InlineGameObject::GetChildren() const
{
	return m_GameObject->GetChildren();
}

void InlineGameObject::MoveChild(IGameObject* child, IGameObjectContainer& new_container)
{
	m_GameObject->MoveChild(child, new_container);
}

void InlineGameObject::SetEnabled(bool enabled)
{
	m_GameObject->SetEnabled(enabled);
}

void InlineGameObject::SetScene(Scene* scene, SceneKey key)
{
	m_GameObject->SetScene(scene, key);
}

std::unique_ptr<IGameObject> InlineGameObject::Copy() const
{
	return m_GameObject->Copy();
}

IUpdateable& InlineGameObject::GetUpdateable()
{
	return m_GameObject->GetUpdateable();
}

const IUpdateable& InlineGameObject::GetUpdateable() const
{
	return m_GameObject->GetUpdateable();
}

ITransformable& InlineGameObject::GetTransformable()
{
	return m_GameObject->GetTransformable();
}

const ITransformable& InlineGameObject::GetTransformable() const
{
	return m_GameObject->GetTransformable();
}