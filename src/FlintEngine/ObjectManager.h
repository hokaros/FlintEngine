#pragma once
#include <Core/GameObject.h>

class IObjectManagerObserver
{
public:
	virtual void OnObjectDestroying(IGameObject& object) = 0;

	virtual ~IObjectManagerObserver() = default;
};

// Obiekt przechowuj¹cy wszystkie obiekty
class ObjectManager
	: public IGameObjectContainer
{
public:
	ObjectManager();
	~ObjectManager();

	// Dodaje obiekt do zarz¹dzanych
	virtual void AddGameObject(std::unique_ptr<IGameObject> object) override;

	void AddToMessageSubscribers(IGameObject* object);

	void AddNewObject(std::unique_ptr<IGameObject> object);
	void DestroyObject(IGameObject* object);
	void DestroyAll();

	void DisposeDestroyed();
	void ActivateNewObjects();

	const std::list<IGameObject*>& GetAllMessageSubscribers() const;
	const std::list<std::unique_ptr<IGameObject>>& GetOwnedObjects() const;
	std::list<std::unique_ptr<IGameObject>>& GetOwnedObjects();

	void Subscribe(IObjectManagerObserver& observer);

	// Usuwa natychmiast wszystkie obiekty
	void Clear();

	void SetAsMain();

	static ObjectManager* Main();
private:
	void DestroyObjectImpl(IGameObject* gameObject, bool detach = true);

	void NotifyObjectDestroying(IGameObject* object);

private:
	std::list<IGameObject*> m_MessageSubscribers;
	std::list<std::unique_ptr<IGameObject>> m_OwnedObjects;
	std::list<IGameObject*> m_DestroyedObjects;
	std::list<IGameObject*> m_NewMessageSubscribers;

	std::vector<IObjectManagerObserver*> m_Observers;

	static ObjectManager* s_Main;
};

