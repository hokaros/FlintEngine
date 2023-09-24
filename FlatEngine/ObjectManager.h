#pragma once
#include "GameObject.h"

// Obiekt przechowuj¹cy wszystkie obiekty
class ObjectManager
{
public:
	ObjectManager();

	// Dodaje obiekt do zarz¹dzanych wraz z ca³ym drzewem dzieci
	void AddObject(GameObject* object);
	// Usuwanie obiektu jest zadaniem kogoœ innego
	void AddUndestroyable(GameObject* object);
	void DestroyObject(GameObject* object, bool detach=true);

	void DisposeDestroyed();

	const std::list<GameObject*>& GetAllObjects() const;

	// Usuwa natychmiast wszystkie obiekty
	void Clear();

	~ObjectManager();

	static ObjectManager* Main();
private:
	std::list<GameObject*> allObjects;
	std::list<GameObject*> destroyables;
	std::list<GameObject*> destroyed;

	static ObjectManager* main;
};

