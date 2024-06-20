#pragma once
#include <Math/Transform.h>
#include "Timer.h"
#include "ObjectComponent.h"
#include "IGameObject.h"
#include "SerializableTypes.h"
#include <functional>
#include <math.h>
#include <mutex>
#include <optional>

#include <list>

class IGameObjectContainer
{
public:
	virtual void AddGameObject(std::unique_ptr<IGameObject>) = 0;

	virtual ~IGameObjectContainer() = default;
};

class GameObject
	: public IGameObject
	, public ITransformable
	, public IUpdateable
{
public:
	GameObject();
	GameObject(const Vector& size);
	GameObject(const Vector& size, const Vector& position);
	// World transform
	GameObject(const Transform& transform);
	GameObject(const GameObject& other);

	std::unique_ptr<GameObject> CopyShallow() const;

	static GameObject* Instantiate(const Vector& size);
	static GameObject* Instantiate(const Vector& size, const Vector& position);
	static GameObject* Instantiate(const GameObject& other);

	static GameObject* Instantiate(IGameObjectContainer& container);

	static void Destroy(GameObject* game_object);

public: /* IGameObject */
	virtual const std::string& GetName() const override;
	virtual void SetName(const std::string& name) override;

	virtual IGameObject* GetParent() const override;
	virtual void SetParent(IGameObject* parent) override;

	virtual const std::vector<std::unique_ptr<IGameObject>>& GetChildren() const override;
	virtual void AddChild(std::unique_ptr<IGameObject> child) override;
	virtual void RemoveChild(IGameObject& child) override;
	virtual void MoveChild(IGameObject* child, IGameObjectContainer& new_container) override;

	virtual const std::vector<std::unique_ptr<ObjectComponent>>& GetAllComponents() const override;

	virtual void SetEnabled(bool enabled) override;

	virtual void SetScene(Scene* scene, SceneKey) override;

	virtual std::unique_ptr<IGameObject> Copy() const override;

	virtual IUpdateable& GetUpdateable() override;
	virtual const IUpdateable& GetUpdateable() const override;
	virtual ITransformable& GetTransformable() override;
	virtual const ITransformable& GetTransformable() const override;

public: /* ITransformable */
	virtual const Vector& GetWorldPosition() const override;
	virtual void SetWorldPosition(const Vector& pos) override;
	virtual Vector GetLocalPosition() const override;
	virtual void SetLocalPosition(const Vector& pos) override;
	// Translate in world space
	virtual void Translate(const Vector& offset) override;

	virtual const Vector& GetWorldScale() const override;
	virtual void SetWorldScale(const Vector& scale) override;
	virtual Vector GetLocalScale() const override;
	virtual void SetLocalScale(const Vector& scale) override;

	virtual float GetWorldRotation() const override;
	virtual void SetWorldRotation(float rot) override;
	virtual float GetLocalRotation() const override;
	// Rotate in world space
	virtual void Rotate(float angle) override;
	// Rotates so that the x-axis of the object is heading towards specified world position
	virtual void LookAt(const Vector& pos) override;

	// Transforms from local space to world space
	virtual Vector TransformPoint(const Vector& local_pos) const override;
	// Transforms from world space to local space
	virtual Vector InvTransformPoint(const Vector& world_pos) const override;

public: /* IUpdateable */
	// Called once per frame
	virtual void Update() override;
	virtual void RenderUpdate(SceneRenderer& renderer) override;

	// Called before the first frame
	virtual void Awake() override;
	// Before the first frame, but after Awake()
	virtual void Start() override;

	virtual void OnDestroy() override;

	//
	std::vector<std::unique_ptr<IGameObject>>& GetChildren(); // TODO: remove

	void AddComponent(std::unique_ptr<ObjectComponent> component);
	void RemoveComponent(ObjectComponent* component);
	void RemoveComponent(size_t component_index);
	std::vector<std::unique_ptr<ObjectComponent>>& GetAllComponents();
	ObjectComponent* GetComponent(size_t idx);
	// Znajduje wszystkie komponenty okreœlonego typu
	template<class T>
	std::list<T*>* FindComponents();
	// Znajduje wszystkie komponenty okreœlonego typu u dzieci
	template<class T>
	std::list<T*>* FindComponentsInChildren(); // TODO: find by RTC

	Vector LookingDirection() const;

	bool IsEnabled() const;

	Vector VectorLocalToWorld(const Vector& localVec) const;

	Scene* GetScene() const;

	~GameObject() = default;

protected:
	std::vector<std::unique_ptr<ObjectComponent>> components;

private:
	bool isEnabled = true;

	std::string name = "GameObject";
	Transform m_Transform;

	IGameObject* m_Parent = nullptr;
	std::vector<std::unique_ptr<IGameObject>> children;

	Scene* scene = nullptr;
};


class GameObjectFactory
{
public:
	std::unique_ptr<GameObject> CreatePrefab();

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

	for (std::unique_ptr<IGameObject>& child : children) 
	{
		GameObject* child_go = dynamic_cast<GameObject*>(child.get());
		if (child_go == nullptr) // TODO: add support for IGameObject
			continue;

		std::list<T*>* foundInChild = child_go->FindComponents<T>();

		for (T* desired : *foundInChild) 
		{
			found->push_back(desired);
		}

		delete foundInChild;
	}

	return found;
}