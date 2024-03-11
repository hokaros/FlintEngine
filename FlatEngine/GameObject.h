#pragma once
#include "Vector.h"
#include "Timer.h"
#include "ObjectComponent.h"
#include "SerializableTypes.h"
#include <functional>
#include <math.h>
#include <mutex>
#include <optional>

#include <list>

class GameObjectSerializer;
class AssetExplorer;
class HierarchyEditor;
class GameObject;

class PrefabCreationKey
{
public:
	PrefabCreationKey(const PrefabCreationKey&) = default;
	PrefabCreationKey(PrefabCreationKey&&) = default;

private:
	PrefabCreationKey() = default;

	friend GameObjectSerializer;
	friend AssetExplorer;
	friend HierarchyEditor;
	friend GameObject;
};

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
	std::vector<std::unique_ptr<ObjectComponent>>& GetAllComponents();
	const std::vector<std::unique_ptr<ObjectComponent>>& GetAllComponents() const;
	ObjectComponent* GetComponent(size_t idx);
	// Znajduje komponent okreœlonego typu
	template<class T>
	T* FindComponent();
	// Znajduje wszystkie komponenty okreœlonego typu
	template<class T>
	std::list<T*>* FindComponents();
	// Znajduje wszystkie komponenty okreœlonego typu u dzieci
	template<class T>
	std::list<T*>* FindComponentsInChildren();
	

	// Raz na klatkê
	void Update();
	void RenderUpdate();
	// Na pocz¹tku gry
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
	// Piksele, które zajmuje ten obiekt
	std::vector<VectorInt>* GetPixels() const;

	void SetEnabled(bool enabled);
	bool IsEnabled() const;

	void SetPosition(const Vector& newPosition);
	void Translate(const Vector& offset); // przesuniêcie
	void SetSize(const Vector& newSize);
	void Rotate(double angle);
	void SetRotation(double rotation);
	// Obraca tak, aby oœ X obiektu by³a skierowana w stronê danego punktu
	void LookAt(const Vector& point);

	Vector LocalToWorld(const Vector& localPos) const;

	void AddChild(std::unique_ptr<GameObject> child);
	void MoveChild(GameObject* child, IGameObjectContainer& new_container);
	const std::vector<std::unique_ptr<GameObject>>& GetChildren() const;
	GameObject* GetParent() const;

	~GameObject() = default;

protected:
	GameObject();
	GameObject(const Vector& size);
	GameObject(const Vector& size, const Vector& position);
	GameObject(const GameObject& other);

protected:
	std::vector<std::unique_ptr<ObjectComponent>> components;

private:
	bool isEnabled = true;

	std::string name = "GameObject";
	Vector size;
	Vector position;
	double rotation = 0.0;

	GameObject* parent = nullptr;
	std::vector<std::unique_ptr<GameObject>> children;
};


class GameObjectFactory
{
public:
	std::unique_ptr<GameObject> CreatePrefab(PrefabCreationKey);

	GameObjectFactory& SetName(const std::string& name);
	GameObjectFactory& SetSize(const Vector& size);
	GameObjectFactory& SetPosition(const Vector& position);

private:
	std::optional<std::string> m_Name;

	std::optional<Vector> m_Size;

	std::optional<Vector> m_Position;
};


class PrefabRef;
template<>
class SerializableTypeInterface<PrefabRef>;

class PrefabRef
{
public:
	PrefabRef() = default;
	PrefabRef(const std::string& file_path);

	const GameObject* Get() const;
	
private:
	std::string m_FilePath;

	friend SerializableTypeInterface<PrefabRef>;
};

template<>
class SerializableTypeInterface<PrefabRef>
{
public:
	static inline void ParseString(const std::string& str, PrefabRef& out_value)
	{
		out_value.m_FilePath = str;
	}
	static inline std::string ToString(const PrefabRef& value)
	{
		return value.m_FilePath;
	}

	STI_DEFINE_TYPECODE_GETTER()
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

	for (std::unique_ptr<GameObject>& child : children) 
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