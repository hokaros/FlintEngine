#include "PrefabInstance.h"

PrefabInstance::PrefabInstance(const GameObject& prefab)
	: m_Prefab(&prefab)
	, m_OriginalComponentCount(prefab.GetAllComponents().size())
	, m_ResultGameObject(std::make_unique<InlineGameObject>())
{
}

GameObject& PrefabInstance::GetResult()
{
	return m_ResultGameObject->GetResult();
}

const GameObject& PrefabInstance::GetResult() const
{
	return m_ResultGameObject->GetResult();
}

void PrefabInstance::Destroy()
{
	m_ResultGameObject->Destroy();
}

void PrefabInstance::SetName(const std::string& name)
{
	m_Name = name;
	m_ResultGameObject->SetName(name);
}

void PrefabInstance::SetSize(const Vector& size)
{
	m_Size = size;
	m_ResultGameObject->SetSize(size);
}

void PrefabInstance::SetPosition(const Vector& position)
{
	m_Position = position;
	m_ResultGameObject->SetPosition(position);
}

void PrefabInstance::AddChild(std::unique_ptr<IEditableGameObject> child)
{
	m_AdditionalChildren.push_back(&child->GetResult());
	m_ResultGameObject->AddChild(std::move(child));
}

void PrefabInstance::AddComponent(std::unique_ptr<ObjectComponent> component)
{
	m_AdditionalComponents.push_back(component.get());
	m_ResultGameObject->AddComponent(std::move(component));
}

void PrefabInstance::RemoveComponent(size_t index)
{
	size_t remaining_original_count = m_OriginalComponentCount - m_RemovedComponents.size();
	if (index >= remaining_original_count)
	{ // The component is non-original
		size_t index_in_additional = index - remaining_original_count;
		m_AdditionalComponents.erase(m_AdditionalComponents.cbegin() + index_in_additional);
	}
	else
	{ // Removing original component
		m_RemovedComponents.push_back(index);
	}

	m_ResultGameObject->RemoveComponent(index);
}

void PrefabInstance::ModifyComponentField(std::unique_ptr<ComponentFieldChange> change)
{
	FE_ASSERT(change != nullptr, "No change passed");

	change->field->SetFieldValue(change->component, change->GetValue());

	m_ComponentFieldChanges.push_back(std::move(change));
}

std::vector<std::unique_ptr<IEditableGameObject>>& PrefabInstance::GetChildren()
{
	return m_ResultGameObject->GetChildren();
}

const std::vector<std::unique_ptr<IEditableGameObject>>& PrefabInstance::GetChildren() const
{
	return m_ResultGameObject->GetChildren();
}

EditableGameObjectType PrefabInstance::GetType() const
{
	return EditableGameObjectType::PrefabInstance;
}

std::unique_ptr<GameObject> PrefabInstance::ToRuntimeObject(std::unique_ptr<PrefabInstance> editable_object)
{
	return InlineGameObject::ToRuntimeObject(std::move(editable_object->m_ResultGameObject));
}
