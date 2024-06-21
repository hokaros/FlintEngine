#pragma once
#include <Editables/InlineGameObject.h>
#include "ComponentSerializer.h"

class GameObjectStringDescProxy
{
public:
	virtual GameObjectType GetType() const = 0;

	virtual ~GameObjectStringDescProxy() = default;
};

struct GameObjectStringDesc
{
	std::map<std::string, std::string> params;
	std::vector<std::unique_ptr<ComponentStringDesc>> components;
	std::vector<std::unique_ptr<GameObjectStringDescProxy>> children;
};

class InlineGameObjectStringDescEndpoint
	: public GameObjectStringDescProxy
{
public:
	InlineGameObjectStringDescEndpoint(std::unique_ptr<GameObjectStringDesc> desc);

	GameObjectStringDesc& GetDesc();
	const GameObjectStringDesc& GetDesc() const;

	virtual GameObjectType GetType() const override;

private:
	std::unique_ptr<GameObjectStringDesc> m_Desc;
};

class GameObjectSerializer
{
public:
	static std::unique_ptr<GameObjectStringDesc> SerializeGameObject(const InlineGameObject& game_object);
	static std::unique_ptr<InlineGameObject> DeserializeGameObject(const GameObjectStringDesc& desc);

	static std::unique_ptr<GameObjectStringDescProxy> SerializeIEditable(const GameObject& game_object);
	static std::unique_ptr<GameObject> DeserializeIEditable(const GameObjectStringDescProxy& desc);

private:
	static std::unique_ptr<InlineGameObject> DeserializePureGameObject(const GameObjectStringDesc& desc);

	static void SerializeComponents(const InlineGameObject& game_object, GameObjectStringDesc& desc);
	static void DeserializeComponents(InlineGameObject& game_object, const GameObjectStringDesc& desc);

	static void SerializeChildren(const InlineGameObject& game_object, GameObjectStringDesc& desc);
	static void DeserializeChildren(InlineGameObject& game_object, const GameObjectStringDesc& desc);
};

