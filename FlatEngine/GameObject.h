#pragma once
#include "Vector.h"
#include "Timer.h"
#include "ObjectComponent.h"
#include <functional>
#include <math.h>
#include <mutex>
#include <optional>

#include <list>
#define BUMPOUT_SPEED 10000.0

class PrefabFactory;
class GameObjectSerializer;
class AssetExplorer;

class PrefabCreationKey
{
public:
	PrefabCreationKey(const PrefabCreationKey&) = default;
	PrefabCreationKey(PrefabCreationKey&&) = default;

private:
	PrefabCreationKey() = default;

	friend PrefabFactory;
	friend GameObjectSerializer;
	friend AssetExplorer;
};

class GameObject;

class IGameObjectContainer
{
public:
	virtual void AddGameObject(std::unique_ptr<GameObject>) = 0;

	virtual ~IGameObjectContainer() = default;
};

class GameObject
{
public:
	bool renderUnseen = false;
	// Czy obiekt mo�e by� niszczony przez pociski
	bool isDestroyable = false;

	std::function<void(GameObject& collider)> onCollision;
public:
	GameObject(PrefabCreationKey);
	GameObject(const Vector& size, PrefabCreationKey);
	GameObject(const Vector& size, const Vector& position, PrefabCreationKey);
	GameObject(const GameObject& other, PrefabCreationKey);

	static GameObject* Instantiate(const Vector& size);
	static GameObject* Instantiate(const Vector& size, const Vector& position);
	static GameObject* Instantiate(const GameObject& other);

	static GameObject* Instantiate(IGameObjectContainer& container);

	static void Destroy(GameObject* game_object);

	void AddComponent(std::unique_ptr<ObjectComponent> component);
	void RemoveComponent(ObjectComponent* component);
	void RemoveComponent(size_t component_index);
	std::list<ObjectComponent*> GetAllComponents();
	std::list<const ObjectComponent*> GetAllComponents() const;
	ObjectComponent* GetComponent(size_t idx);
	// Znajduje komponent okre�lonego typu
	template<class T>
	T* FindComponent();
	// Znajduje wszystkie komponenty okre�lonego typu
	template<class T>
	std::list<T*>* FindComponents();
	// Znajduje wszystkie komponenty okre�lonego typu u dzieci
	template<class T>
	std::list<T*>* FindComponentsInChildren();
	

	// Raz na klatk�
	void Update();
	void RenderUpdate();
	// Na pocz�tku gry
	void Start();
	// Przed Start()
	void Awake();
	void OnDestroy();

	const std::string& GetName() const;
	void SetName(const std::string& name);

	const Vector& GetPosition() const;
	const Vector& GetSize() const;
	double GetRotation() const;
	Vector LookingDirection() const;
	Vector GetMiddle() const;
	// Piksele, w kt�rych ten obiekt ma collidera
	std::vector<VectorInt>* GetPixels() const;

	// Odwracalne zniszczenie obiektu
	void SetDestroyed(bool destroyed);
	void SetEnabled(bool enabled);
	bool IsDestroyed() const;
	bool IsEnabled() const;
	void SubscribeDestroyed(function<void(GameObject*)> handler);

	void SetPosition(const Vector& newPosition);
	void Translate(const Vector& offset); // przesuni�cie
	void SetSize(const Vector& newSize);
	void Rotate(double angle);
	void SetRotation(double rotation);
	// Obraca tak, aby o� X obiektu by�a skierowana w stron� danego punktu
	void LookAt(const Vector& point);

	Vector LocalToWorld(const Vector& localPos) const;

	void AddChild(GameObject* child);
	void RemoveChild(GameObject* child);
	const std::list<GameObject*>& GetChildren() const;
	GameObject* GetParent() const;

	~GameObject() = default; // TODO: we should also delete child objects

protected:
	GameObject();
	GameObject(const Vector& size);
	GameObject(const Vector& size, const Vector& position);
	GameObject(const GameObject& other);

protected:
	std::list<std::unique_ptr<ObjectComponent>> components;

	bool insideOutCollision = false;

private:
	bool isEnabled = true;

	std::string name = "GameObject";
	Vector size;
	Vector position;
	double rotation = 0.0;

	double asyncRotation = 0.0;

	GameObject* parent = NULL;
	std::list<GameObject*> children;

	std::list<function<void(GameObject*)>> onDestroyedChanged;
};


class GameObjectFactory
{
public:
	std::unique_ptr<GameObject> CreatePrefab(PrefabCreationKey);

	GameObjectFactory& SetName(const std::string& name);
	GameObjectFactory& SetSize(const Vector& size);
	GameObjectFactory& SetPosition(const Vector& position);

private:
	std::unique_ptr<GameObject> CreatePrefabSizePosition(PrefabCreationKey);

private:
	std::optional<std::string> m_Name;

	std::optional<Vector> m_Size;

	std::optional<Vector> m_Position;
};



template<class T>
T* GameObject::FindComponent() {
	for (std::unique_ptr<ObjectComponent>& component : components) 
	{
		T* desired = dynamic_cast<T*>(component.get());
		if (desired != nullptr) 
		{
			return desired;
		}
	}
	return NULL;
}

template<class T>
std::list<T*>* GameObject::FindComponents() 
{
	std::list<T*>* found = new std::list<T*>();

	for (std::unique_ptr<ObjectComponent>& component : components) 
	{
		T* desired = dynamic_cast<T*>(component.get());
		if (desired != nullptr) 
		{
			found->push_back(desired);
		}
	}

	return found;
}

template<class T>
std::list<T*>* GameObject::FindComponentsInChildren() 
{
	std::list<T*>* found = new std::list<T*>();

	for (GameObject* child : children) 
	{
		std::list<T*>* foundInChild = child->FindComponents<T>();

		for (T* desired : *foundInChild) 
		{
			found->push_back(desired);
		}

		delete foundInChild;
	}

	return found;
}