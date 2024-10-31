#pragma once
#include <Core/GameObject.h>

class IObjectManagerObserver
{
public:
	virtual void OnObjectDestroying(GameObject& object) = 0;

	virtual ~IObjectManagerObserver() = default;
};

// Obiekt przechowuj�cy wszystkie obiekty
class ObjectManager
	: public IGameObjectContainer
{
public:
	ObjectManager();
	~ObjectManager();

	// Dodaje obiekt do zarz�dzanych
	virtual void AddGameObject(std::unique_ptr<GameObject> object) override;
	virtual const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const override;

	void AddToMessageSubscribers(GameObject* object);

	void AddNewObject(std::unique_ptr<GameObject> object);
	void DestroyObject(GameObject* object);
	void DestroyAll();

	void DisposeDestroyed();
	void ActivateNewObjects();

	const std::list<GameObject*>& GetAllMessageSubscribers() const;
	const std::vector<std::unique_ptr<GameObject>>& GetOwnedObjects() const;
	std::vector<std::unique_ptr<GameObject>>& GetOwnedObjects();

	void Subscribe(IObjectManagerObserver& observer);

	// Usuwa natychmiast wszystkie obiekty
	void Clear();

	void SetAsMain();

	static ObjectManager* Main();
private:
	void DestroyObjectImpl(GameObject* gameObject, bool detach = true);

	void NotifyObjectDestroying(GameObject* object);

private:
	std::list<GameObject*> m_MessageSubscribers;
	std::vector<std::unique_ptr<GameObject>> m_OwnedObjects;
	std::list<GameObject*> m_DestroyedObjects;
	std::list<GameObject*> m_NewMessageSubscribers;

	std::vector<IObjectManagerObserver*> m_Observers;

	static ObjectManager* s_Main;
};

