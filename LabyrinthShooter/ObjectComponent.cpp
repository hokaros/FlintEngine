#include "ObjectComponent.h"
ObjectComponent::ObjectComponent(GameObject& owner)
	: gameObject(owner) {

}

GameObject& ObjectComponent::GetOwner() const {
	return gameObject;
}

IUpdateable* ObjectComponent::Copy() {
	return Copy(gameObject);
}