#pragma once
#include "../FlatEngine/GameObject.h"

// Kolizje wyst�puj� na zewn�trz, nie w �rodku obiektu
class Cage :
	public GameObject
{
public:
	Cage(const Vector& size);
	Cage(const Vector& size, const Vector& position);
};

