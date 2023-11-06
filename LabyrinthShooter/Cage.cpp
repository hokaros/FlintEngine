#include "Cage.h"

Cage::Cage(const Vector& size) 
	: GameObject(size) {
	insideOutCollision = true;
}

Cage::Cage(const Vector& size, const Vector& position)
	: GameObject(size, position) {
	insideOutCollision = true;
}
