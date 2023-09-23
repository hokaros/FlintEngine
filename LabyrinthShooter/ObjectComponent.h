#pragma once
#include <SDL.h>
#include "IUpdateable.h"
#include "GameObject.h"

class ObjectComponent : public IUpdateable
{
public:
	ObjectComponent(GameObject& owner);

	GameObject& GetOwner() const;

	IUpdateable* Copy() override;
	virtual ObjectComponent* Copy(GameObject& newOwner) = 0;

protected:
	GameObject& gameObject;
};

