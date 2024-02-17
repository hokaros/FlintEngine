#pragma once
#include "GameObject.h"
#include "ComponentSerializer.h"

struct GameObjectStringDesc
{
	std::map<std::string, std::string> params;
	std::vector<std::unique_ptr<ComponentStringDesc>> components;
	std::vector<GameObjectStringDesc> children;
};

class GameObjectSerializer
{
public:
	static std::unique_ptr<GameObjectStringDesc> SerializeGameObject(const GameObject& game_object);
	static std::unique_ptr<GameObject> DeserializeGameObject(const GameObjectStringDesc& desc);

private:
	static void SerializeComponents(const GameObject& game_object, GameObjectStringDesc& desc);

	static std::unique_ptr<GameObject> DeserializePureGameObject(const GameObjectStringDesc& desc);
	static void DeserializeComponents(GameObject& game_object, const GameObjectStringDesc& desc);
};

