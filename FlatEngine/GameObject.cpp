#include "GameObject.h"
#include "ObjectComponent.h"
#include "ObjectManager.h"
#include "AssetManager.h"

GameObject::GameObject()
	: size(1, 1) 
{
}

GameObject::GameObject(const Vector& size)
	: size(size) 
{
}

GameObject::GameObject(const Vector& size, const Vector& position)
	: position(position), size(size) 
{
}

GameObject::GameObject(PrefabCreationKey)
	: GameObject() 
{
}

GameObject::GameObject(const Vector& size, PrefabCreationKey)
	: GameObject(size) 
{
}

GameObject::GameObject(const Vector& size, const Vector& position, PrefabCreationKey)
	: GameObject(size, position) 
{
}

GameObject::GameObject(const GameObject& other, PrefabCreationKey)
	: GameObject(other)
{
}

GameObject::GameObject(const GameObject& other) 
	: GameObject(other.size, other.position, {}) 
{
	// Skopiowanie komponentów
	for (const std::unique_ptr<ObjectComponent>& component : other.components)
	{
		std::unique_ptr<ObjectComponent> cmpCpy = component->Copy();
		AddComponent(std::move(cmpCpy));
	}

	// Skopiowanie dzieci
	for (GameObject* child : other.children) 
	{
		GameObject* childCopy = new GameObject(*child);

		AddChild(childCopy);
	}
}

GameObject* GameObject::Instantiate(const Vector& size)
{
	ObjectManager* object_manager = ObjectManager::Main();

	GameObject* game_object = new GameObject(size);
	object_manager->AddObject(std::unique_ptr<GameObject>(game_object));

	return game_object;
}

GameObject* GameObject::Instantiate(const Vector& size, const Vector& position)
{
	ObjectManager* object_manager = ObjectManager::Main();

	GameObject* game_object = new GameObject(size, position);
	object_manager->AddObject(std::unique_ptr<GameObject>(game_object));

	return game_object;
}

GameObject* GameObject::Instantiate(const GameObject& other)
{
	GameObject* game_object = new GameObject(other);
	ObjectManager::Main()->AddObject(std::unique_ptr<GameObject>(game_object));

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
	components.remove_if([&component](const std::unique_ptr<ObjectComponent>& go_comp) 
	{
		return go_comp.get() == component;
	});
}

void GameObject::RemoveComponent(size_t component_index)
{
	RemoveComponent(GetComponent(component_index));
}

std::list<ObjectComponent*> GameObject::GetAllComponents()
{
	std::list<ObjectComponent*> out_components;

	for (std::unique_ptr<ObjectComponent>& component : components)
	{
		out_components.push_back(component.get());
	}

	return out_components;
}

std::list<const ObjectComponent*> GameObject::GetAllComponents() const
{
	std::list<const ObjectComponent*> out_components;

	for (const std::unique_ptr<ObjectComponent>& component : components)
	{
		out_components.push_back(component.get());
	}

	return out_components;
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
}

void GameObject::RenderUpdate() 
{
	if (!isEnabled)
		return;

	for (std::unique_ptr<ObjectComponent>& component : components)
	{
		component->RenderUpdate();
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
}

void GameObject::Awake()
{
	if (!isEnabled)
		return;

	for (std::unique_ptr<ObjectComponent>& component : components)
	{
		component->Awake();
	}
}

void GameObject::OnDestroy()
{
	for (std::unique_ptr<ObjectComponent>& component : components)
	{
		component->OnDestroy();
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

const Vector& GameObject::GetSize() const 
{
	return size;
}

double GameObject::GetRotation() const 
{
	return rotation;
}

Vector GameObject::LookingDirection() const 
{
	return Vector(
		cos(rotation * M_PI / 180),
		sin(rotation * M_PI / 180)
	);
}

const Vector& GameObject::GetPosition() const 
{
	return position;
}

Vector GameObject::GetMiddle() const 
{
	return position + size / 2;
}

void GameObject::SetPosition(const Vector& newPosition) 
{
	Vector offset = newPosition - position;
	position = newPosition;

	for (GameObject* child : children) 
	{
		child->Translate(offset);
	}
}

void GameObject::Translate(const Vector& offset) 
{
	position += offset;

	for (GameObject* child : children) 
	{
		child->Translate(offset);
	}
}

void GameObject::SetSize(const Vector& newSize) 
{
	Vector sizeChange(newSize.x / size.x, newSize.y / size.y);

	size.x = newSize.x;
	size.y = newSize.y;

	// Rozmiar dzieci
	for (GameObject* child : children) 
	{
		Vector childNewSize(child->size.x * sizeChange.x, child->size.y * sizeChange.y);
		child->SetSize(childNewSize);
	}
}

void GameObject::Rotate(double angle) 
{
	double prevRot = rotation;
	double newRot = rotation + angle;
	double newRotRadians = newRot * M_PI / 180;

	Vector middle = GetMiddle();
	
	for (GameObject* child : children) 
	{
		child->Rotate(angle);

		Vector childMid = child->GetMiddle();
		double radius = (middle - childMid).Length();

		Vector childNewPos(
			cos(newRotRadians) * radius,
			sin(newRotRadians) * radius
		);
		childNewPos += middle;
		Vector dPos = childNewPos - childMid;

		child->Translate(dPos);
	}


	rotation += angle;
}

void GameObject::SetRotation(double newRot) 
{
	double dRot = newRot - rotation;
	Rotate(dRot);
}

void GameObject::LookAt(const Vector& point) 
{
	Vector toPoint = point - GetMiddle();
	double lookRotation = atan2(toPoint.y, toPoint.x) * 180 / M_PI;

	double dRot = lookRotation - rotation;
	Rotate(dRot);
}

Vector GameObject::LocalToWorld(const Vector& localPos) const 
{
	Vector fromMid = localPos - size / 2;
	//double radius = fromMid.Length();
	fromMid.Rotate(rotation * M_PI / 180);

	Vector rotatedSize = size;
	rotatedSize.Rotate(rotation * M_PI / 180);

	return fromMid + position + rotatedSize / 2;
	/*double localAngle = atan2(fromMid.y, fromMid.x) * 180 / M_PI;
	double targetAngle = localAngle - rotation;

	Vector unrotatedPos(
		cos(targetAngle * M_PI / 180) * radius,
		sin(targetAngle * M_PI / 180) * radius
	);*/
	//return unrotatedPos + position + rotatedSize / 2;
}

void GameObject::AddChild(GameObject* child) 
{
	child->parent = this;

	children.push_back(child);
}

void GameObject::RemoveChild(GameObject* child) 
{
	child->parent = NULL;

	children.remove(child);
}

const std::list<GameObject*>& GameObject::GetChildren() const 
{
	return children;
}

GameObject* GameObject::GetParent() const 
{
	return parent;
}

std::vector<VectorInt>* GameObject::GetPixels() const 
{
	std::vector<VectorInt>* pixels = new std::vector<VectorInt>();

	for (int x = 0; x < size.x; x++) 
	{
		for (int y = 0; y < size.y; y++) 
		{
			pixels->push_back(position + Vector(x, y));
		} 
	}

	return pixels;
}

void GameObject::SetDestroyed(bool destroyed) 
{
	isEnabled = !destroyed;

	for (function<void(GameObject*)> handler : onDestroyedChanged) 
	{
		if (handler)
			handler(this);
	}
}

void GameObject::SetEnabled(bool enabled) 
{
	isEnabled = enabled;

	for (GameObject* child : children) 
	{
		child->SetEnabled(enabled);
	}
}

bool GameObject::IsDestroyed() const 
{
	return !isEnabled;
}

bool GameObject::IsEnabled() const 
{
	return isEnabled;
}

void GameObject::SubscribeDestroyed(function<void(GameObject*)> handler) 
{
	onDestroyedChanged.push_back(handler);
}

std::unique_ptr<GameObject> GameObjectFactory::CreatePrefab(PrefabCreationKey key)
{
	std::unique_ptr<GameObject> prefab = CreatePrefabSizePosition(key);

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

std::unique_ptr<GameObject> GameObjectFactory::CreatePrefabSizePosition(PrefabCreationKey key)
{
	if (m_Size.has_value() && m_Position.has_value())
	{
		return std::make_unique<GameObject>(m_Size.value(), m_Position.value(), key);
	}
	else if (m_Size.has_value())
	{
		return std::make_unique<GameObject>(m_Size.value(), key);
	}

	return std::make_unique<GameObject>(key);
}

PrefabRef::PrefabRef(const std::string& file_path)
	: m_FilePath(file_path)
{
}

const GameObject* PrefabRef::Get() const
{
	return AssetManager::GetInstance()->GetPrefab(m_FilePath);
}
