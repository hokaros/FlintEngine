#pragma once
#include <memory>
#include <vector>

class GameObject;

class IGameObjectContainer
{
public:
	virtual void AddGameObject(std::unique_ptr<GameObject>) = 0;
	virtual const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const = 0; // TODO: this enforces specific implementation of storing GameObjects

	virtual ~IGameObjectContainer() = default;
};