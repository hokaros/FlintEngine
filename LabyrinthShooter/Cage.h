#pragma once
#include "GameObject.h"

// Kolizje wystêpuj¹ na zewn¹trz, nie w œrodku obiektu
class Cage :
	public GameObject
{
public:
	Cage(const Vector& size, const std::list<GameObject*>& allObjects);
	Cage(const Vector& size, const Vector& position, const std::list<GameObject*>& allObjects);
};

