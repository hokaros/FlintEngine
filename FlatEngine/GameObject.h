#pragma once
#include "Vector.h"
#include "Timer.h"
#include "IUpdateable.h"
#include <functional>
#include <math.h>
#include <mutex>

#include <list>
#define BUMPOUT_SPEED 10000.0

class Game;
class PrefabFactory;
class GameObjectSerializer;

class PrefabCreationKey
{
private:
	PrefabCreationKey() = default;
	PrefabCreationKey(const PrefabCreationKey&) = default;
	PrefabCreationKey(PrefabCreationKey&&) = default;

	friend Game; // TODO: let's not
	friend PrefabFactory;
	friend GameObjectSerializer;
};

class GameObject
{
public:
	bool renderUnseen = false;
	// Czy obiekt mo¿e byæ niszczony przez pociski
	bool isDestroyable = false;

	std::function<void(GameObject& collider)> onCollision;
public:
	GameObject(PrefabCreationKey);
	GameObject(const Vector& size, PrefabCreationKey);
	GameObject(const Vector& size, const Vector& position, PrefabCreationKey);

	static GameObject* Instantiate(const Vector& size);
	static GameObject* Instantiate(const Vector& size, const Vector& position);
	static GameObject* Instantiate(const GameObject& other);
	static void Destroy(GameObject* game_object);

	void AddComponent(IUpdateable* component);
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

	const Vector& GetPosition() const;
	const Vector& GetSize() const;
	double GetRotation() const;
	Vector LookingDirection() const;
	Vector GetMiddle() const;
	// Piksele, w których ten obiekt ma collidera
	std::vector<VectorInt>* GetPixels() const;

	// Odwracalne zniszczenie obiektu
	void SetDestroyed(bool destroyed);
	void SetEnabled(bool enabled);
	bool IsDestroyed() const;
	bool IsEnabled() const;
	void SubscribeDestroyed(function<void(GameObject*)> handler);

	void SetPosition(const Vector& newPosition);
	void Translate(const Vector& offset); // przesuniêcie
	void SetSize(const Vector& newSize);
	void Rotate(double angle);
	void SetRotation(double rotation);
	// Obraca tak, aby oœ X obiektu by³a skierowana w stronê danego punktu
	void LookAt(const Vector& point);

	Vector LocalToWorld(const Vector& localPos) const;

	void AddChild(GameObject* child);
	void RemoveChild(GameObject* child);
	const std::list<GameObject*>& GetChildren() const;
	GameObject* GetParent() const;


	~GameObject();
protected:
	GameObject();
	GameObject(const Vector& size);
	GameObject(const Vector& size, const Vector& position);
	GameObject(const GameObject& other);

protected:
	std::list<IUpdateable*> components;

	bool insideOutCollision = false;
private:
	bool isEnabled = true;

	Vector size;
	Vector position;
	double rotation = 0.0;

	double asyncRotation = 0.0;

	GameObject* parent = NULL;
	std::list<GameObject*> children;

	std::list<function<void(GameObject*)>> onDestroyedChanged;
};



template<class T>
T* GameObject::FindComponent() {
	for (IUpdateable* component : components) {
		T* desired = dynamic_cast<T*>(component);
		if (desired != NULL) {
			return desired;
		}
	}
	return NULL;
}

template<class T>
std::list<T*>* GameObject::FindComponents() {
	std::list<T*>* found = new std::list<T*>();

	for (IUpdateable* component : components) {
		T* desired = dynamic_cast<T*>(component);
		if (desired != NULL) {
			found->push_back(desired);
		}
	}

	return found;
}

template<class T>
std::list<T*>* GameObject::FindComponentsInChildren() {
	std::list<T*>* found = new std::list<T*>();

	for (GameObject* child : children) {
		std::list<T*>* foundInChild = child->FindComponents<T>();

		for (T* desired : *foundInChild) {
			found->push_back(desired);
		}

		delete foundInChild;
	}

	return found;
}