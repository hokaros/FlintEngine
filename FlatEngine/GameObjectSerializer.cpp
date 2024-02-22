#include "GameObjectSerializer.h"
#include "SerializableTypes.h"

static constexpr const char* s_GameObjectNameFieldName = "m_Name";
static constexpr const char* s_GameObjectSizeFieldName = "m_Size";
static constexpr const char* s_GameObjectPositionFieldName = "m_Pos";

std::unique_ptr<GameObject> GameObjectSerializer::DeserializeGameObject(const GameObjectStringDesc& desc)
{
	std::unique_ptr<GameObject> game_object = DeserializePureGameObject(desc);
	DeserializeComponents(*game_object, desc);
	DeserializeChildren(*game_object, desc);

	return game_object;
}

std::unique_ptr<GameObjectStringDesc> GameObjectSerializer::SerializeGameObject(const GameObject& game_object)
{
	std::unique_ptr<GameObjectStringDesc> go_serialized = std::make_unique<GameObjectStringDesc>();

	go_serialized->params.insert({ s_GameObjectNameFieldName, game_object.GetName() });
	go_serialized->params.insert({ s_GameObjectSizeFieldName, STI<Vector>::ToString(game_object.GetSize()) });
	go_serialized->params.insert({ s_GameObjectPositionFieldName, STI<Vector>::ToString(game_object.GetPosition()) });

	SerializeComponents(game_object, *go_serialized);
	SerializeChildren(game_object, *go_serialized);

	return go_serialized;
}

void GameObjectSerializer::SerializeComponents(const GameObject& game_object, GameObjectStringDesc& desc)
{
	for (const std::unique_ptr<ObjectComponent>& component : game_object.GetAllComponents())
	{
		desc.components.push_back(ComponentSerializer::SerializeComponent(*component));
	}
}

std::unique_ptr<GameObject> GameObjectSerializer::DeserializePureGameObject(const GameObjectStringDesc& desc)
{
	GameObjectFactory prefab_factory;

	auto size_it = desc.params.find(s_GameObjectSizeFieldName);
	if (size_it != desc.params.end())
	{
		Vector size;
		STI<Vector>::ParseString(size_it->second, size);

		prefab_factory.SetSize(size);
	}

	auto pos_it = desc.params.find(s_GameObjectPositionFieldName);
	if (pos_it != desc.params.end())
	{
		Vector pos;
		STI<Vector>::ParseString(pos_it->second, pos);

		prefab_factory.SetPosition(pos);
	}

	auto name_it = desc.params.find(s_GameObjectNameFieldName);
	if (name_it != desc.params.end())
	{
		prefab_factory.SetName(name_it->second);
	}

	return prefab_factory.CreatePrefab(PrefabCreationKey());
}

void GameObjectSerializer::DeserializeComponents(GameObject& game_object, const GameObjectStringDesc& desc)
{
	for (const std::unique_ptr<ComponentStringDesc>& component_desc : desc.components)
	{
		std::unique_ptr<ObjectComponent> component = ComponentSerializer::DeserializeComponent(*component_desc);
		if (component != nullptr)
		{
			game_object.AddComponent(std::move(component));
		}
	}
}

void GameObjectSerializer::SerializeChildren(const GameObject& game_object, GameObjectStringDesc& desc)
{
	for (const std::unique_ptr<GameObject>& child : game_object.GetChildren())
	{
		desc.children.push_back(SerializeGameObject(*child));
	}
}

void GameObjectSerializer::DeserializeChildren(GameObject& game_object, const GameObjectStringDesc& desc)
{
	for (const std::unique_ptr<GameObjectStringDesc>& child : desc.children)
	{
		game_object.AddChild(DeserializeGameObject(*child));
	}
}
