#pragma once
#include "../FlatEngine/GameObject.h"

// Kolizje wystêpuj¹ na zewn¹trz, nie w œrodku obiektu
class Cage :
	public GameObject
{
public:
	Cage(const Vector& size);
	Cage(const Vector& size, const Vector& position);
};

