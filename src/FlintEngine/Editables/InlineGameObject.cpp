#include "InlineGameObject.h"

InlineGameObject::InlineGameObject()
	: m_GameObject(std::make_unique<GameObject>())
{
}

InlineGameObject::InlineGameObject(const GameObject& other)
	: m_GameObject(other.CopyShallow())
{
	for (const std::unique_ptr<IGameObject>& child : other.GetChildren())
	{
		const GameObject* child_go = dynamic_cast<const GameObject*>(child.get()); // TODO: remove dynamic cast
		if (child_go != nullptr)
		{
			std::unique_ptr<InlineGameObject> editable_copy = std::make_unique<InlineGameObject>(*child_go);
			m_GameObject->AddChild(std::move(editable_copy));
		}
		else
		{
			FE_ASSERT(dynamic_cast<const IEditableGameObject*>(child.get()) != nullptr, "Child should be editable");
			m_GameObject->AddChild(child->Copy());
		}
	}
}

InlineGameObject::InlineGameObject(const InlineGameObject& other)
	: InlineGameObject(*other.m_GameObject)
{

}

GameObject& InlineGameObject::GetResult_Depr()
{
	return *m_GameObject;
}

const GameObject& InlineGameObject::GetResult_Depr() const
{
	return *m_GameObject;
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

EditableGameObjectType InlineGameObject::Serializable_GetType() const
{
	return EditableGameObjectType::InlineGameObject;
}

std::unique_ptr<GameObject> InlineGameObject::ToRuntimeObject(std::unique_ptr<InlineGameObject> editable_object)
{
	if (editable_object == nullptr)
		return nullptr;

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

void InlineGameObject::AddChild(std::unique_ptr<IGameObject> child)
{
	m_GameObject->AddChild(std::move(child));
}

void InlineGameObject::RemoveChild(IGameObject& child)
{
	m_GameObject->RemoveChild(child);
}

void InlineGameObject::MoveChild(IGameObject* child, IGameObjectContainer& new_container)
{
	m_GameObject->MoveChild(child, new_container);
}

const std::vector<std::unique_ptr<ObjectComponent>>& InlineGameObject::GetAllComponents() const
{
	return m_GameObject->GetAllComponents();
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
	return std::make_unique<InlineGameObject>(*m_GameObject);
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