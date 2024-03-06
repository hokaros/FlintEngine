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

	std::unique_ptr<ObjectComponent> Copy();
	virtual RuntimeTypeCode GetTypeCode() const = 0;

protected:
	void SetOwner(GameObject* owner);

protected:
	GameObject* m_GameObject;

	friend GameObject;
};

