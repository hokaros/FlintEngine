#pragma once
#include <SDL.h>
#include "IUpdateable.h"
#include <memory>

class GameObject;

class ObjectComponent : public IUpdateable
{
public:
	GameObject& GetOwner() const;

	virtual std::unique_ptr<ObjectComponent> Copy() = 0;

protected:
	void SetOwner(GameObject* owner);

protected:
	GameObject* m_GameObject;

	friend GameObject;
};

