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
	static std::unique_ptr<GameObject> DeserializeGameObject(const GameObjectStringDesc& desc);
	static std::unique_ptr<GameObjectStringDesc> SerializeGameObject(const GameObject& game_object);

private:
	static std::unique_ptr<GameObject> DeserializePureGameObject(const GameObjectStringDesc& desc);
	static void DeserializeComponents(GameObject& game_object, const GameObjectStringDesc& desc);
};

