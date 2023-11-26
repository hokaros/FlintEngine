#pragma once
#include <SDL.h>
#include <memory>
#include "IUpdateable.h"
#include "RuntimeTypeCode.h"

class GameObject;

class ObjectComponent : public IUpdateable
{
public:
	GameObject& GetOwner() const;

	virtual std::unique_ptr<ObjectComponent> Copy() = 0;
	virtual RuntimeTypeCode GetTypeCode() = 0;

protected:
	void SetOwner(GameObject* owner);

protected:
	GameObject* m_GameObject;

	friend GameObject;
};

