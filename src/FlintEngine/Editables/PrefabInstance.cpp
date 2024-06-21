#include "PrefabInstance.h"
#include "AssetManager.h"
#include <ComponentFieldDefinition.h>

PrefabInstance::PrefabInstance(const std::string& prefab_path)
	: GameObject(*AssetManager::GetInstance()->GetPrefab(prefab_path))
	, m_PrefabPath(prefab_path)
{
	const GameObject* prefab = AssetManager::GetInstance()->GetPrefab(m_PrefabPath);
	FE_ASSERT(prefab != nullptr, "No prefab");

	m_OriginalComponentCount = prefab->GetAllComponents().size();
}

PrefabInstance::PrefabInstance(const PrefabInstance& other)
	: GameObject(other)
	, m_PrefabPath(other.m_PrefabPath)
	, m_Name(other.m_Name)
	, m_Size(other.m_Size)
	, m_Position(other.m_Position)
	// TODO: additional children
	, m_OriginalComponentCount(other.m_OriginalComponentCount)
	// TODO: additional components
	// TODO: removed components
	// TODO: component field changes
{

}

std::unique_ptr<GameObject> PrefabInstance::ToRuntimeObject(std::unique_ptr<PrefabInstance> editable_object)
{
	return std::move(editable_object);
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
	return GameObject::GetName();
}

void PrefabInstance::SetName(const std::string& name)
{
	m_Name = name;
	GameObject::SetName(name);
}

GameObject* PrefabInstance::GetParent() const
{
	return GameObject::GetParent();
}

void PrefabInstance::SetParent(GameObject* parent)
{
	GameObject::SetParent(parent);
}

const std::vector<std::unique_ptr<GameObject>>& PrefabInstance::GetChildren() const
{
	return GameObject::GetChildren();
}

void PrefabInstance::AddChild(std::unique_ptr<GameObject> child)
{
	// TODO: add to additional children
	GameObject::AddChild(std::move(child));
}

void PrefabInstance::RemoveChild(GameObject& child)
{
	// TODO: remove from additional children
	GameObject::RemoveChild(child);
}

void PrefabInstance::MoveChild(GameObject* child, IGameObjectContainer& new_container)
{
	GameObject::MoveChild(child, new_container);
}

const std::vector<std::unique_ptr<ObjectComponent>>& PrefabInstance::GetAllComponents() const
{
	return GameObject::GetAllComponents();
}


void PrefabInstance::AddComponent(std::unique_ptr<ObjectComponent> component)
{
	m_AdditionalComponents.push_back(component.get());
	GameObject::AddComponent(std::move(component));
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

	GameObject::RemoveComponent(index);
}

void PrefabInstance::ModifyComponentField(std::unique_ptr<ComponentFieldChange> change)
{
	FE_ASSERT(change != nullptr, "No change passed");

	change->field->SetFieldValue(change->component, change->GetValue());
}

void PrefabInstance::SetEnabled(bool enabled)
{
	GameObject::SetEnabled(enabled);
}

void PrefabInstance::SetScene(Scene* scene, SceneKey key)
{
	GameObject::SetScene(scene, key);
}

std::unique_ptr<GameObject> PrefabInstance::Copy() const
{
	PrefabInstance* cpy = new PrefabInstance(*this);

	return std::unique_ptr<GameObject>(cpy);
}

IUpdateable& PrefabInstance::GetUpdateable()
{
	return GameObject::GetUpdateable();
}

const IUpdateable& PrefabInstance::GetUpdateable() const
{
	return GameObject::GetUpdateable();
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
	return GameObject::GetWorldPosition();
}

void PrefabInstance::SetWorldPosition(const Vector& pos)
{
	m_Position = pos;
	GameObject::SetWorldPosition(pos);
}

Vector PrefabInstance::GetLocalPosition() const
{
	return GameObject::GetLocalPosition();
}

void PrefabInstance::SetLocalPosition(const Vector& pos)
{
	GameObject::SetLocalPosition(pos);
	m_Position = GetWorldPosition();
}

void PrefabInstance::Translate(const Vector& offset)
{
	GameObject::Translate(offset);
}

const Vector& PrefabInstance::GetWorldScale() const
{
	return GameObject::GetWorldScale();
}

void PrefabInstance::SetWorldScale(const Vector& scale)
{
	m_Size = scale;
	GameObject::SetWorldScale(scale);
}

Vector PrefabInstance::GetLocalScale() const
{
	return GameObject::GetLocalScale();
}

void PrefabInstance::SetLocalScale(const Vector& scale)
{
	GameObject::SetLocalScale(scale);
	m_Size = GetWorldScale();
}

float PrefabInstance::GetWorldRotation() const
{
	return GameObject::GetWorldRotation();
}

void PrefabInstance::SetWorldRotation(float rot)
{
	// TODO: serialize rotation
	GameObject::SetWorldRotation(rot);
}

float PrefabInstance::GetLocalRotation() const
{
	return GameObject::GetLocalRotation();
}

void PrefabInstance::Rotate(float angle)
{
	// TODO: serialize rotation
	GameObject::Rotate(angle);
}

void PrefabInstance::LookAt(const Vector& pos)
{
	// TODO: serialize rotation
	GameObject::LookAt(pos);
}


Vector PrefabInstance::TransformPoint(const Vector& local_pos) const
{
	return GameObject::TransformPoint(local_pos);
}

Vector PrefabInstance::InvTransformPoint(const Vector& world_pos) const
{
	return GameObject::InvTransformPoint(world_pos);
}