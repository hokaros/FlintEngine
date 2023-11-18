#include "ObjectComponent.h"

GameObject& ObjectComponent::GetOwner() const 
{
	return *m_GameObject;
}

void ObjectComponent::SetOwner(GameObject* owner)
{
	m_GameObject = owner;
}
