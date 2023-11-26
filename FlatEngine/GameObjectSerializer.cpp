#include "GameObjectSerializer.h"
#include "SerializableTypes.h"

static constexpr const char* s_GameObjectSizeFieldName = "m_Size";
static constexpr const char* s_GameObjectPositionFieldName = "m_Pos";

std::unique_ptr<GameObject> GameObjectSerializer::DeserializeGameObject(const GameObjectStringDesc& desc)
{
	std::unique_ptr<GameObject> game_object = DeserializePureGameObject(desc);
	DeserializeComponents(*game_object, desc);

	return game_object;
}

std::unique_ptr<GameObjectStringDesc> GameObjectSerializer::SerializeGameObject(const GameObject& game_object)
{
	std::unique_ptr<GameObjectStringDesc> go_serialized = std::make_unique<GameObjectStringDesc>();

	go_serialized->params.insert({ s_GameObjectSizeFieldName, STI<Vector>::ToString(game_object.GetSize()) });
	go_serialized->params.insert({ s_GameObjectPositionFieldName, STI<Vector>::ToString(game_object.GetPosition()) });

	// TODO: component serialization

	return go_serialized;
}

std::unique_ptr<GameObject> GameObjectSerializer::DeserializePureGameObject(const GameObjectStringDesc& desc)
{
	Vector size;
	bool is_size_present = false;
	Vector pos;
	bool is_pos_present = false;

	auto size_it = desc.params.find(s_GameObjectSizeFieldName);
	if (size_it != desc.params.end())
	{
		STI<Vector>::ParseString(size_it->second, size);
		is_size_present = true;
	}

	auto pos_it = desc.params.find(s_GameObjectPositionFieldName);
	if (pos_it != desc.params.end())
	{
		STI<Vector>::ParseString(pos_it->second, pos);
		is_pos_present = true;
	}

	if (is_size_present && is_pos_present)
	{
		return std::make_unique<GameObject>(size, pos, PrefabCreationKey());
	}
	else if (is_size_present)
	{
		return std::make_unique<GameObject>(size, PrefabCreationKey());
	}

	return std::make_unique<GameObject>(PrefabCreationKey());
}

void GameObjectSerializer::DeserializeComponents(GameObject& game_object, const GameObjectStringDesc& desc)
{
	for (const ComponentStringDesc& component_desc : desc.components)
	{
		std::unique_ptr<ObjectComponent> component = ComponentSerializer::DeserializeComponent(component_desc);
		if (component != nullptr)
		{
			game_object.AddComponent(std::move(component));
		}
	}
}
