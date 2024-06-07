#include "GameObject.h"
#include "ObjectComponent.h"
#include "ObjectManager.h"
#include "AssetManager.h"

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
		std::unique_ptr<GameObject> childCopy = std::make_unique<GameObject>(*child);

		AddChild(std::move(childCopy));
	}
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
		child->Update();
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
		child->RenderUpdate(renderer);
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
		child->Start();
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
		child->Awake();
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
		child->OnDestroy();
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

const Vector& GameObject::GetWorldPosition() const
{
	return m_Transform.GetPosition();
}

Vector GameObject::GetLocalPosition() const
{
	const Vector& self_world_pos = m_Transform.GetPosition();

	// Position in the parent space
	if (parent != nullptr)
	{
		return parent->m_Transform.InvTransformPoint(self_world_pos);
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
	// TODO: implement
	FE_ASSERT(false, "Implement");
	return Vector::ZERO;
}

float GameObject::GetWorldRotation() const
{
	return m_Transform.GetRotation();
}

float GameObject::GetLocalRotation() const
{
	// Rotation in parent space
	// TODO: implement
	FE_ASSERT(false, "Implement");
	return 0.0f;
}

void GameObject::SetWorldPosition(const Vector& newPosition)
{
	m_Transform.SetPosition(newPosition);
}

void GameObject::SetLocalPosition(const Vector& newPosition)
{
	Vector wanted_world_pos = newPosition;

	if (parent != nullptr)
	{
		// Set position in the parent's coordinates
		wanted_world_pos = parent->m_Transform.TransformPoint(newPosition);
	}

	const Vector translation = wanted_world_pos - GetWorldPosition();
	Translate(translation);
}

void GameObject::Translate(const Vector& offset) 
{
	m_Transform.Translate(offset);

	// Moving the children
	for (std::unique_ptr<GameObject>& child : children)
	{
		child->Translate(offset);
	}
}

void GameObject::SetWorldScale(const Vector& newScale) 
{
	const Vector& size = m_Transform.GetScale();
	Vector sizeChange(newScale.x / size.x, newScale.y / size.y);

	m_Transform.SetScale(newScale);

	// Rozmiar dzieci
	for (std::unique_ptr<GameObject>& child : children)
	{
		const Vector& child_size = child->GetWorldScale();
		Vector childNewSize(child_size.x * sizeChange.x, child_size.y * sizeChange.y);
		child->SetWorldScale(childNewSize);
	}
}

void GameObject::SetLocalScale(const Vector& newScale)
{
	FE_ASSERT(false, "Implement");
}

void GameObject::Rotate(float angle) 
{
	float prevRot = m_Transform.GetRotation();
	float newRot = prevRot + angle;
	float newRotRadians = newRot * M_PI / 180;

	Vector middle = m_Transform.GetPosition();
	
	for (std::unique_ptr<GameObject>& child : children)
	{
		child->Rotate(angle);

		Vector childMid = child->GetLocalPosition();
		double radius = (middle - childMid).Length();

		Vector childNewPos(
			cos(newRotRadians) * radius,
			sin(newRotRadians) * radius
		);
		childNewPos += middle;
		Vector dPos = childNewPos - childMid;

		child->Translate(dPos);
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

Vector GameObject::LocalToWorld(const Vector& localPos) const 
{
	return m_Transform.TransformPoint(localPos);
}

void GameObject::AddChild(std::unique_ptr<GameObject> child)
{
	child->parent = this;

	children.push_back(std::move(child));
}

void GameObject::MoveChild(GameObject* child, IGameObjectContainer& new_container) 
{
	child->parent = nullptr;

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
	return parent;
}

Scene* GameObject::GetScene() const
{
	return scene;
}

void GameObject::SetScene(Scene* scene, SceneKey)
{
	this->scene = scene;
}

std::vector<VectorInt>* GameObject::GetPixels() const 
{
	std::vector<VectorInt>* pixels = new std::vector<VectorInt>();

	const Vector& size = m_Transform.GetScale();
	const Vector& position = m_Transform.GetPosition();

	for (int x = 0; x < size.x; x++) 
	{
		for (int y = 0; y < size.y; y++) 
		{
			pixels->push_back(position + Vector(x, y));
		} 
	}

	return pixels;
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
