#include "Cage.h"

Cage::Cage(const Vector& size, const std::list<GameObject*>& allObjects) 
	: GameObject(size, allObjects) {
	insideOutCollision = true;
}

Cage::Cage(const Vector& size, const Vector& position, const std::list<GameObject*>& allObjects)
	: GameObject(size, position, allObjects) {
	insideOutCollision = true;
}
