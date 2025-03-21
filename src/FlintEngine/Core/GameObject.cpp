#include "GameObject.h"
#include "ObjectComponent.h"
#include <ObjectManager.h>
#include <AssetManager.h>
#include <ComponentFieldDefinition.h>

GameObject::GameObject()
	: GameObject(Vector(1,1))
{
}

GameObject::GameObject(const Vector& size)
	: GameObject(size, Vector(0,0))
{
}

GameObject::GameObject(const Vector& size, const Vector& position)
	: m_Transform(position, size)
{
}

GameObject::GameObject(const Transform& transform)
	: m_Transform(transform)
{
}

GameObject::GameObject(const GameObject& other) 
	: m_Transform(other.m_Transform)
	, name(other.name)
{
	// Skopiowanie komponentów
	for (const std::unique_ptr<ObjectComponent>& component : other.components)
	{
		std::unique_ptr<ObjectComponent> cmpCpy = component->Copy();
		AddComponent(std::move(cmpCpy));
	}

	// Skopiowanie dzieci
	for (const std::unique_ptr<GameObject>& child : other.children) 
	{
		std::unique_ptr<GameObject> childCopy = child->Copy();
		AddChild(std::move(childCopy));
	}
}

std::unique_ptr<GameObject> GameObject::CopyShallow() const
{
	std::unique_ptr<GameObject> new_object = std::make_unique<GameObject>(m_Transform);
	new_object->name = name;

	// Skopiowanie komponentów
	for (const std::unique_ptr<ObjectComponent>& component : components)
	{
		std::unique_ptr<ObjectComponent> cmpCpy = component->Copy();
		new_object->AddComponent(std::move(cmpCpy));
	}

	return new_object;
}

GameObject* GameObject::Instantiate(const Vector& size)
{
	ObjectManager* object_manager = ObjectManager::Main();

	GameObject* game_object = new GameObject(size);
	object_manager->AddNewObject(std::unique_ptr<GameObject>(game_object));

	return game_object;
}

GameObject* GameObject::Instantiate(const Vector& size, const Vector& position)
{
	ObjectManager* object_manager = ObjectManager::Main();

	GameObject* game_object = new GameObject(size, position);
	object_manager->AddNewObject(std::unique_ptr<GameObject>(game_object));

	return game_object;
}

GameObject* GameObject::Instantiate(const GameObject& other)
{
	GameObject* game_object = new GameObject(other);
	ObjectManager::Main()->AddNewObject(std::unique_ptr<GameObject>(game_object));

	return game_object;
}

GameObject* GameObject::Instantiate(IGameObjectContainer& container)
{
	GameObject* game_object = new GameObject();
	container.AddGameObject(std::unique_ptr<GameObject>(game_object));

	return game_object;
}

void GameObject::Destroy(GameObject* game_object)
{
	ObjectManager::Main()->DestroyObject(game_object);
}

void GameObject::RemoveChild(GameObject& child)
{
	for (auto it = children.begin(); it != children.end(); it++)
	{
		std::unique_ptr<GameObject>& c = *it;
		if (c.get() == &child)
		{
			children.erase(it);
			return;
		}
	}
}

std::vector<std::unique_ptr<GameObject>>& GameObject::GetChildren()
{
	return children;
}

void GameObject::AddComponent(std::unique_ptr<ObjectComponent> component)
{
	component->SetOwner(this);
	components.push_back(std::move(component));
}

void GameObject::RemoveComponent(ObjectComponent* component)
{
	for (auto it = components.begin(); it != components.end(); it++)
	{
		std::unique_ptr<ObjectComponent>& my_comp = *it;
		if (my_comp.get() == component)
		{
			components.erase(it);
			break;
		}
	}
}

void GameObject::RemoveComponent(size_t component_index)
{
	RemoveComponent(GetComponent(component_index));
}

void GameObject::ModifyComponentField(std::unique_ptr<ComponentFieldChange> change)
{
	FE_ASSERT(change != nullptr, "No change passed");

	change->field->SetFieldValue(change->component, change->GetValue());
}

std::vector<std::unique_ptr<ObjectComponent>>& GameObject::GetAllComponents()
{
	return components;
}

const std::vector<std::unique_ptr<ObjectComponent>>& GameObject::GetAllComponents() const
{
	return components;
}

ObjectComponent* GameObject::GetComponent(size_t idx)
{
	size_t i = 0;
	for (std::unique_ptr<ObjectComponent>& component : components)
	{
		if (i == idx)
		{
			return component.get();
		}

		i++;
	}

	return nullptr;
}

void GameObject::Update() 
{
	if (!isEnabled)
		return;

	for (std::unique_ptr<ObjectComponent>& component : components)
	{
		component->Update();
	}

	for (std::unique_ptr<GameObject>& child : children)
	{
		child->GetUpdateable().Update();
	}
}

void GameObject::RenderUpdate(SceneRenderer& renderer)
{
	if (!isEnabled)
		return;

	for (std::unique_ptr<ObjectComponent>& component : components)
	{
		component->RenderUpdate(renderer);
	}

	for (std::unique_ptr<GameObject>& child : children)
	{
		child->GetUpdateable().RenderUpdate(renderer);
	}
}

void GameObject::RenderEditorSelected(SceneRenderer& renderer)
{
	if (!isEnabled)
		return;

	for (std::unique_ptr<ObjectComponent>& component : components)
	{
		component->RenderEditorSelected(renderer);
	}

	for (std::unique_ptr<GameObject>& child : children)
	{
		child->GetUpdateable().RenderEditorSelected(renderer);
	}
}

void GameObject::Start() 
{
	if (!isEnabled)
		return;

	for (std::unique_ptr<ObjectComponent>& component : components)
	{
		component->Start();
	}

	for (std::unique_ptr<GameObject>& child : children)
	{
		child->GetUpdateable().Start();
	}
}

void GameObject::Awake()
{
	if (!isEnabled)
		return;

	for (std::unique_ptr<ObjectComponent>& component : components)
	{
		component->Awake();
	}

	for (std::unique_ptr<GameObject>& child : children)
	{
		child->GetUpdateable().Awake();
	}
}

void GameObject::OnDestroy()
{
	for (std::unique_ptr<ObjectComponent>& component : components)
	{
		component->OnDestroy();
	}

	for (std::unique_ptr<GameObject>& child : children)
	{
		child->GetUpdateable().OnDestroy();
	}
}

const std::string& GameObject::GetName() const
{
	return name;
}

void GameObject::SetName(const std::string& name)
{
	this->name = name;
}

void GameObject::SetParent(GameObject* parent)
{
	m_Parent = parent;
}

const Vector& GameObject::GetWorldPosition() const
{
	return m_Transform.GetPosition();
}

Vector GameObject::GetLocalPosition() const
{
	const Vector& self_world_pos = m_Transform.GetPosition();

	// Position in the parent space
	if (m_Parent != nullptr)
	{
		return m_Parent->GetTransformable().InvTransformPoint(self_world_pos);
	}

	return self_world_pos;
}

Vector GameObject::LookingDirection() const 
{
	return m_Transform.GetLookDir();
}

const Vector& GameObject::GetWorldScale() const
{
	return m_Transform.GetScale();
}

Vector GameObject::GetLocalScale() const
{
	// Scale in parent space
	if (m_Parent == nullptr)
	{
		return m_Transform.GetScale();
	}
	else
	{
		const Vector& parent_scale = m_Parent->GetWorldScale();
		return m_Transform.GetScale().GetScaled(Vector(1 / parent_scale.x, 1 / parent_scale.y));
	}
}

float GameObject::GetWorldRotation() const
{
	return m_Transform.GetRotation();
}

float GameObject::GetLocalRotation() const
{
	// Rotation in parent space
	if (m_Parent == nullptr)
	{
		return m_Transform.GetRotation();
	}
	else
	{
		const float parent_rot = m_Parent->GetWorldRotation();
		return m_Transform.GetRotation() - parent_rot;
	}
}

void GameObject::SetWorldPosition(const Vector& newPosition)
{
	const Vector wanted_translation = newPosition - GetWorldPosition();
	Translate(wanted_translation);
}

void GameObject::SetLocalPosition(const Vector& newPosition)
{
	Vector wanted_world_pos = newPosition;

	if (m_Parent != nullptr)
	{
		// Set position in the parent's coordinates
		wanted_world_pos = m_Parent->GetTransformable().TransformPoint(newPosition);
	}

	const Vector translation = wanted_world_pos - GetWorldPosition();
	Translate(translation);
}

std::unique_ptr<GameObject> GameObject::Copy() const
{
	return std::make_unique<GameObject>(*this);
}

IUpdateable& GameObject::GetUpdateable()
{
	return *this;
}

const IUpdateable& GameObject::GetUpdateable() const
{
	return *this;
}

ITransformable& GameObject::GetTransformable()
{
	return *this;
}

const ITransformable& GameObject::GetTransformable() const
{
	return *this;
}

void GameObject::Translate(const Vector& offset) 
{
	m_Transform.Translate(offset);

	// Moving the children
	for (std::unique_ptr<GameObject>& child : children)
	{
		child->GetTransformable().Translate(offset);
	}
}

void GameObject::SetWorldScale(const Vector& new_scale) 
{
	const Vector& size = m_Transform.GetScale();
	Vector sizeChange(new_scale.x / size.x, new_scale.y / size.y);

	m_Transform.SetScale(new_scale);

	// Rozmiar dzieci
	for (std::unique_ptr<GameObject>& child : children)
	{
		const Vector child_prev_size = child->GetTransformable().GetWorldScale();
		const Vector child_new_size = child_prev_size.GetScaled(sizeChange);
		child->GetTransformable().SetWorldScale(child_new_size);

		const Vector prev_to_child = child->GetWorldPosition() - GetWorldPosition();
		const Vector new_to_child = prev_to_child.GetScaled(sizeChange);
		const Vector d_to_child = new_to_child - prev_to_child;
		child->Translate(d_to_child);
	}
}

void GameObject::SetLocalScale(const Vector& new_scale)
{
	// Scale in parent space
	if (m_Parent == nullptr)
	{
		SetWorldScale(new_scale);
	}
	else
	{
		const Vector& parent_scale = m_Parent->GetWorldScale();
		const Vector target_world_scale = new_scale.GetScaled(parent_scale);
		SetWorldScale(target_world_scale);
	}
}

void GameObject::Rotate(float angle) 
{
	float prevRot = m_Transform.GetRotation();
	float newRot = prevRot + angle;
	float newRotRadians = newRot * M_PI / 180;

	Vector middle = m_Transform.GetPosition();
	
	for (std::unique_ptr<GameObject>& child : children)
	{
		child->GetTransformable().Rotate(angle);

		const Vector local_pos = child->GetTransformable().GetLocalPosition();
		const Vector target_local_pos = child->GetTransformable().GetLocalPosition().GetRotated(DegToRad(angle));
		child->GetTransformable().SetLocalPosition(target_local_pos);
	}

	m_Transform.Rotate(angle);
}

void GameObject::SetWorldRotation(float newRot) 
{
	float dRot = newRot - m_Transform.GetRotation();
	Rotate(dRot);
}

void GameObject::LookAt(const Vector& point) 
{
	Vector toPoint = point - GetWorldPosition();
	double lookRotation = atan2(toPoint.y, toPoint.x) * 180 / M_PI;

	double dRot = lookRotation - m_Transform.GetRotation();
	Rotate(dRot);
}

Vector GameObject::TransformPoint(const Vector& local_pos) const
{
	return m_Transform.TransformPoint(local_pos);
}

Vector GameObject::InvTransformPoint(const Vector& world_pos) const
{
	return m_Transform.InvTransformPoint(world_pos);
}

Vector GameObject::VectorLocalToWorld(const Vector& localVec) const
{
	return TransformPoint(localVec) - TransformPoint(Vector::ZERO);
}

Vector GameObject::VectorWorldToLocal(const Vector& worldVec) const
{
	return InvTransformPoint(worldVec) - InvTransformPoint(Vector::ZERO);
}

void GameObject::AddChild(std::unique_ptr<GameObject> child)
{
	child->SetParent(this);

	children.push_back(std::move(child));
}

void GameObject::MoveChild(GameObject* child, IGameObjectContainer& new_container) 
{
	child->SetParent(nullptr);

	for (auto it = children.begin(); it != children.end(); it++)
	{
		std::unique_ptr<GameObject>& it_val = *it;
		if (it_val.get() == child)
		{
			new_container.AddGameObject(std::move(it_val));
			children.erase(it);
			break;
		}
	}
}

const std::vector<std::unique_ptr<GameObject>>& GameObject::GetChildren() const 
{
	return children;
}

GameObject* GameObject::GetParent() const 
{
	return m_Parent;
}

Scene* GameObject::GetScene() const
{
	return scene;
}

void GameObject::SetScene(Scene* scene, SceneKey)
{
	this->scene = scene;
}

void GameObject::SetEnabled(bool enabled) 
{
	isEnabled = enabled;

	for (std::unique_ptr<GameObject>& child : children)
	{
		child->SetEnabled(enabled);
	}
}

bool GameObject::IsEnabled() const 
{
	return isEnabled;
}

std::unique_ptr<GameObject> GameObjectFactory::CreatePrefab()
{
	std::unique_ptr<GameObject> prefab = std::make_unique<GameObject>();

	if (m_Position.has_value())
	{
		prefab->SetWorldPosition(m_Position.value());
	}

	if (m_Size.has_value())
	{
		prefab->SetWorldScale(m_Size.value());
	}

	if (m_Name.has_value())
	{
		prefab->SetName(m_Name.value());
	}

	return prefab;
}

GameObjectFactory& GameObjectFactory::SetName(const std::string& name)
{
	m_Name = name;

	return *this;
}

GameObjectFactory& GameObjectFactory::SetSize(const Vector& size)
{
	m_Size = size;

	return *this;
}

GameObjectFactory& GameObjectFactory::SetPosition(const Vector& position)
{
	m_Position = position;

	return *this;
}

PrefabRef::PrefabRef(const std::string& file_path)
	: m_FilePath(file_path)
{
}

const GameObject* PrefabRef::Get() const
{
	return AssetManager::GetInstance()->GetPrefab(m_FilePath);
}
