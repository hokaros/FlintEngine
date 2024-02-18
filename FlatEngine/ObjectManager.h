#pragma once
#include "GameObject.h"

// Obiekt przechowuj¹cy wszystkie obiekty
class ObjectManager
	: public IGameObjectContainer
{
public:
	ObjectManager();

	// Dodaje obiekt do zarz¹dzanych
	virtual void AddGameObject(std::unique_ptr<GameObject> object) override;

	void AddToMessageSubscribers(GameObject* object);

	void AddNewObject(std::unique_ptr<GameObject> object);
	void DestroyObject(GameObject* object);

	void DisposeDestroyed();
	void ActivateNewObjects();

	const std::list<GameObject*>& GetAllMessageSubscribers() const;

	// Usuwa natychmiast wszystkie obiekty
	void Clear();

	~ObjectManager();

	static ObjectManager* Main();
private:
	void DestroyObjectImpl(GameObject* gameObject, bool detach = true);

private:
	std::list<GameObject*> m_MessageSubscribers;
	std::list<std::unique_ptr<GameObject>> m_OwnedObjects;
	std::list<GameObject*> m_DestroyedObjects;
	std::list<GameObject*> m_NewMessageSubscribers;

	static ObjectManager* s_Main;
};

