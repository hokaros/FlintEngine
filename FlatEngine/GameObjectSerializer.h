#pragma once
#include "GameObject.h"
#include "ComponentSerializer.h"

struct GameObjectStringDesc
{
	std::map<std::string, std::string> params;
	std::vector<ComponentStringDesc> components;
};

class GameObjectSerializer
{
public:
	static GameObject* DeserializeGameObject(const GameObjectStringDesc& desc);

private:
	static GameObject* DeserializePureGameObject(const GameObjectStringDesc& desc);
	static void DeserializeComponents(GameObject* game_object, const GameObjectStringDesc& desc);
};

