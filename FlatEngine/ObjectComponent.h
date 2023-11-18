#pragma once
#include <SDL.h>
#include "IUpdateable.h"
#include "GameObject.h"

class GameObject;

class ObjectComponent : public IUpdateable
{
public:
	GameObject& GetOwner() const;

protected:
	void SetOwner(GameObject* owner);

protected:
	GameObject* m_GameObject;

	friend GameObject;
};

