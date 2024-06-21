#include "GameObjectSerializer.h"

#include "SerializableTypes.h"
#include "PrefabInstanceSerializer.h"

static constexpr const char* s_GameObjectNameFieldName = "m_Name";
static constexpr const char* s_GameObjectSizeFieldName = "m_Size";
static constexpr const char* s_GameObjectPositionFieldName = "m_Pos";

std::unique_ptr<InlineGameObject> GameObjectSerializer::DeserializeGameObject(const GameObjectStringDesc& desc)
{
	std::unique_ptr<InlineGameObject> game_object = DeserializePureGameObject(desc);
	DeserializeComponents(*game_object, desc);
	DeserializeChildren(*game_object, desc);

	return game_object;
}

std::unique_ptr<GameObjectStringDesc> GameObjectSerializer::SerializeGameObject(const InlineGameObject& game_object)
{
	std::unique_ptr<GameObjectStringDesc> go_serialized = std::make_unique<GameObjectStringDesc>();

	go_serialized->params.insert({ s_GameObjectNameFieldName, game_object.GetName() });
	go_serialized->params.insert({ s_GameObjectSizeFieldName, STI<Vector>::ToString(game_object.GetTransformable().GetWorldScale()) });
	go_serialized->params.insert({ s_GameObjectPositionFieldName, STI<Vector>::ToString(game_object.GetTransformable().GetWorldPosition()) });

	SerializeComponents(game_object, *go_serialized);
	SerializeChildren(game_object, *go_serialized);

	return go_serialized;
}

void GameObjectSerializer::SerializeComponents(const InlineGameObject& game_object, GameObjectStringDesc& desc)
{
	for (const std::unique_ptr<ObjectComponent>& component : game_object.GetAllComponents())
	{
		desc.components.push_back(ComponentSerializer::SerializeComponent(*component));
	}
}

std::unique_ptr<InlineGameObject> GameObjectSerializer::DeserializePureGameObject(const GameObjectStringDesc& desc)
{
	std::unique_ptr<InlineGameObject> game_object = std::make_unique<InlineGameObject>();

	auto size_it = desc.params.find(s_GameObjectSizeFieldName);
	if (size_it != desc.params.end())
	{
		Vector size;
		STI<Vector>::ParseString(size_it->second, size);

		game_object->GetTransformable().SetWorldScale(size);
	}

	auto pos_it = desc.params.find(s_GameObjectPositionFieldName);
	if (pos_it != desc.params.end())
	{
		Vector pos;
		STI<Vector>::ParseString(pos_it->second, pos);

		game_object->GetTransformable().SetWorldPosition(pos);
	}

	auto name_it = desc.params.find(s_GameObjectNameFieldName);
	if (name_it != desc.params.end())
	{
		game_object->SetName(name_it->second);
	}

	return game_object;
}

void GameObjectSerializer::DeserializeComponents(InlineGameObject& game_object, const GameObjectStringDesc& desc)
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

void GameObjectSerializer::SerializeChildren(const InlineGameObject& game_object, GameObjectStringDesc& desc)
{
	for (const std::unique_ptr<GameObject>& child : game_object.GetChildren())
	{
		desc.children.push_back(SerializeIEditable(*child));
	}
}

void GameObjectSerializer::DeserializeChildren(InlineGameObject& game_object, const GameObjectStringDesc& desc)
{
	for (const std::unique_ptr<GameObjectStringDescProxy>& child : desc.children)
	{
		game_object.AddChild(DeserializeIEditable(*child));
	}
}

std::unique_ptr<GameObjectStringDescProxy> GameObjectSerializer::SerializeIEditable(const GameObject& game_object)
{
	switch (game_object.GetGameObjectType())
	{
	case GameObjectType::GameObject:
	{
		const InlineGameObject& inline_object = static_cast<const InlineGameObject&>(game_object);
		return std::make_unique<InlineGameObjectStringDescEndpoint>(SerializeGameObject(inline_object));
	}
	case GameObjectType::PrefabInstance:
	{
		const PrefabInstance& prefab_instance = static_cast<const PrefabInstance&>(game_object);
		return std::make_unique<PrefabInstanceStringDescEndpoint>(PrefabInstanceSerializer::Serialize(prefab_instance));
	}
	default:
		FE_ASSERT(false, "Unimplemented proxying");
		return std::make_unique<InlineGameObjectStringDescEndpoint>(nullptr);
	}
}

std::unique_ptr<GameObject> GameObjectSerializer::DeserializeIEditable(const GameObjectStringDescProxy& desc)
{
	switch (desc.GetType())
	{
	case GameObjectType::GameObject:
	{
		const InlineGameObjectStringDescEndpoint& inline_endpoint = static_cast<const InlineGameObjectStringDescEndpoint&>(desc);
		return DeserializeGameObject(inline_endpoint.GetDesc());
	}
	case GameObjectType::PrefabInstance:
	{
		const PrefabInstanceStringDescEndpoint& prefab_instance = static_cast<const PrefabInstanceStringDescEndpoint&>(desc);
		return PrefabInstanceSerializer::Deserialize(prefab_instance.GetDesc());
	}
	default:
		FE_ASSERT(false, "Unimplemented proxying");
		return std::unique_ptr<GameObject>();
	}
}



InlineGameObjectStringDescEndpoint::InlineGameObjectStringDescEndpoint(std::unique_ptr<GameObjectStringDesc> desc)
	: m_Desc(std::move(desc))
{
}

GameObjectStringDesc& InlineGameObjectStringDescEndpoint::GetDesc()
{
	return *m_Desc;
}

const GameObjectStringDesc& InlineGameObjectStringDescEndpoint::GetDesc() const
{
	return *m_Desc;
}

GameObjectType InlineGameObjectStringDescEndpoint::GetType() const
{
	return GameObjectType::GameObject;
}