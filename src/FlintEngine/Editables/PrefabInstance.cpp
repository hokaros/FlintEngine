#include "PrefabInstance.h"
#include "AssetManager.h"

PrefabInstance::PrefabInstance(const std::string& prefab_path)
	: m_PrefabPath(prefab_path)
{
	const GameObject* prefab = AssetManager::GetInstance()->GetPrefab(m_PrefabPath);
	FE_ASSERT(prefab != nullptr, "No prefab");

	m_OriginalComponentCount = prefab->GetAllComponents().size();
	m_ResultGameObject = std::make_unique<InlineGameObject>(*prefab);
}

GameObject& PrefabInstance::GetResult_Depr()
{
	return m_ResultGameObject->GetResult_Depr();
}

const GameObject& PrefabInstance::GetResult_Depr() const
{
	return m_ResultGameObject->GetResult_Depr();
}

void PrefabInstance::Serializable_AddChild(std::unique_ptr<IEditableGameObject> child)
{
	m_AdditionalChildren.push_back(child.get());
	m_ResultGameObject->Serializable_AddChild(std::move(child));
}

void PrefabInstance::Serializable_RemoveChild(IEditableGameObject& child)
{
	m_ResultGameObject->Serializable_RemoveChild(child);
}

void PrefabInstance::Serializable_AddComponent(std::unique_ptr<ObjectComponent> component)
{
	m_AdditionalComponents.push_back(component.get());
	m_ResultGameObject->Serializable_AddComponent(std::move(component));
}

void PrefabInstance::Serializable_RemoveComponent(size_t index)
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

	m_ResultGameObject->Serializable_RemoveComponent(index);
}

void PrefabInstance::Serializable_ModifyComponentField(std::unique_ptr<ComponentFieldChange> change)
{
	FE_ASSERT(change != nullptr, "No change passed");

	change->field->SetFieldValue(change->component, change->GetValue());

	m_ComponentFieldChanges.push_back(std::move(change));
}

std::vector<std::unique_ptr<IGameObject>>& PrefabInstance::Serializable_GetChildren()
{
	return m_ResultGameObject->Serializable_GetChildren();
}

const std::vector<std::unique_ptr<IGameObject>>& PrefabInstance::Serializable_GetChildren() const
{
	return m_ResultGameObject->Serializable_GetChildren();
}

EditableGameObjectType PrefabInstance::Serializable_GetType() const
{
	return EditableGameObjectType::PrefabInstance;
}

std::unique_ptr<GameObject> PrefabInstance::ToRuntimeObject(std::unique_ptr<PrefabInstance> editable_object)
{
	return InlineGameObject::ToRuntimeObject(std::move(editable_object->m_ResultGameObject));
}

const std::string& PrefabInstance::GetPrefabPath() const
{
	return m_PrefabPath;
}

const std::optional<std::string>& PrefabInstance::GetNameOverride() const
{
	return m_Name;
}

const std::optional<Vector>& PrefabInstance::GetSizeOverride() const
{
	return m_Size;
}

const std::optional<Vector>& PrefabInstance::GetPositionOverride() const
{
	return m_Position;
}

const std::vector<ObjectComponent*>& PrefabInstance::GetAdditionalComponents() const
{
	return m_AdditionalComponents;
}


const std::string& PrefabInstance::GetName() const
{
	return m_ResultGameObject->GetName();
}

void PrefabInstance::SetName(const std::string& name)
{
	m_Name = name;
	m_ResultGameObject->SetName(name);
}

IGameObject* PrefabInstance::GetParent() const
{
	return m_ResultGameObject->GetParent();
}

void PrefabInstance::SetParent(IGameObject* parent)
{
	m_ResultGameObject->SetParent(parent);
}

const std::vector<std::unique_ptr<IGameObject>>& PrefabInstance::GetChildren() const
{
	return m_ResultGameObject->GetChildren();
}

void PrefabInstance::AddChild(std::unique_ptr<IGameObject> child)
{
	// TODO: add to additional children
	m_ResultGameObject->AddChild(std::move(child));
}

void PrefabInstance::RemoveChild(IGameObject& child)
{
	// TODO: remove from additional children
	m_ResultGameObject->RemoveChild(child);
}

void PrefabInstance::MoveChild(IGameObject* child, IGameObjectContainer& new_container)
{
	m_ResultGameObject->MoveChild(child, new_container);
}

void PrefabInstance::SetEnabled(bool enabled)
{
	m_ResultGameObject->SetEnabled(enabled);
}

void PrefabInstance::SetScene(Scene* scene, SceneKey key)
{
	m_ResultGameObject->SetScene(scene, key);
}

std::unique_ptr<IGameObject> PrefabInstance::Copy() const
{
	std::unique_ptr<PrefabInstance> cpy = std::make_unique<PrefabInstance>(m_PrefabPath);
	cpy->m_ResultGameObject = std::make_unique<InlineGameObject>(*m_ResultGameObject);

	return cpy;
}

IUpdateable& PrefabInstance::GetUpdateable()
{
	return m_ResultGameObject->GetUpdateable();
}

const IUpdateable& PrefabInstance::GetUpdateable() const
{
	return m_ResultGameObject->GetUpdateable();
}

ITransformable& PrefabInstance::GetTransformable()
{
	return *this;
}

const ITransformable& PrefabInstance::GetTransformable() const
{
	return *this;
}


const Vector& PrefabInstance::GetWorldPosition() const
{
	return m_ResultGameObject->GetTransformable().GetWorldPosition();
}

void PrefabInstance::SetWorldPosition(const Vector& pos)
{
	m_Position = pos;
	m_ResultGameObject->GetTransformable().SetWorldPosition(pos);
}

Vector PrefabInstance::GetLocalPosition() const
{
	return m_ResultGameObject->GetTransformable().GetLocalPosition();
}

void PrefabInstance::SetLocalPosition(const Vector& pos)
{
	m_ResultGameObject->GetTransformable().SetLocalPosition(pos);
	m_Position = m_ResultGameObject->GetTransformable().GetWorldPosition();
}

void PrefabInstance::Translate(const Vector& offset)
{
	const Vector result_pos = GetWorldPosition() + offset;
	SetWorldPosition(result_pos);
}

const Vector& PrefabInstance::GetWorldScale() const
{
	return m_ResultGameObject->GetTransformable().GetWorldScale();
}

void PrefabInstance::SetWorldScale(const Vector& scale)
{
	m_Size = scale;
	m_ResultGameObject->GetTransformable().SetWorldScale(scale);
}

Vector PrefabInstance::GetLocalScale() const
{
	return m_ResultGameObject->GetTransformable().GetLocalScale();
}

void PrefabInstance::SetLocalScale(const Vector& scale)
{
	m_ResultGameObject->GetTransformable().SetLocalScale(scale);
	m_Size = m_ResultGameObject->GetTransformable().GetWorldScale();
}

float PrefabInstance::GetWorldRotation() const
{
	return m_ResultGameObject->GetTransformable().GetWorldRotation();
}

void PrefabInstance::SetWorldRotation(float rot)
{
	// TODO: serialize rotation
	m_ResultGameObject->GetTransformable().SetWorldRotation(rot);
}

float PrefabInstance::GetLocalRotation() const
{
	return m_ResultGameObject->GetTransformable().GetLocalRotation();
}

void PrefabInstance::Rotate(float angle)
{
	// TODO: serialize rotation
	m_ResultGameObject->GetTransformable().Rotate(angle);
}

void PrefabInstance::LookAt(const Vector& pos)
{
	// TODO: serialize rotation
	m_ResultGameObject->GetTransformable().LookAt(pos);
}


Vector PrefabInstance::TransformPoint(const Vector& local_pos) const
{
	return m_ResultGameObject->GetTransformable().TransformPoint(local_pos);
}

Vector PrefabInstance::InvTransformPoint(const Vector& world_pos) const
{
	return m_ResultGameObject->GetTransformable().InvTransformPoint(world_pos);
}