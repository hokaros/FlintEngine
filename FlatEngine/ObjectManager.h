#pragma once
#include "GameObject.h"

// Obiekt przechowuj�cy wszystkie obiekty
class ObjectManager
{
public:
	ObjectManager();

	// Dodaje obiekt do zarz�dzanych wraz z ca�ym drzewem dzieci
	void AddObject(std::unique_ptr<GameObject> object);
	// Usuwanie obiektu jest zadaniem kogo� innego
	void AddUndestroyable(GameObject* object);
	void DestroyObject(GameObject* object);

	void DisposeDestroyed();
	void ActivateNewObjects();

	const std::list<GameObject*>& GetAllObjects() const;

	// Usuwa natychmiast wszystkie obiekty
	void Clear();

	~ObjectManager();

	static ObjectManager* Main();
private:
	void DestroyObjectImpl(GameObject* gameObject, bool detach = true);

private:
	std::list<GameObject*> m_AllObjects;
	std::list<std::unique_ptr<GameObject>> m_OwnedObjects;
	std::list<GameObject*> m_DestroyedObjects;
	std::list<GameObject*> m_NewObjects;

	static ObjectManager* s_Main;
};

