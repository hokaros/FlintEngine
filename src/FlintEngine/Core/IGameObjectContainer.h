#pragma once
#include <memory>

class GameObject;

class IGameObjectContainer
{
public:
	virtual void AddGameObject(std::unique_ptr<GameObject>) = 0;

	virtual ~IGameObjectContainer() = default;
};