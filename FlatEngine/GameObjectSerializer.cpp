#include "GameObjectSerializer.h"
#include "SerializableTypes.h"

static constexpr const char* s_GameObjectSizeFieldName = "m_Size";
static constexpr const char* s_GameObjectPositionFieldName = "m_Pos";

GameObject* GameObjectSerializer::DeserializeGameObject(const GameObjectStringDesc& desc)
{
	GameObject* game_object = DeserializePureGameObject(desc);
	DeserializeComponents(game_object, desc);

	return game_object;
}

GameObject* GameObjectSerializer::DeserializePureGameObject(const GameObjectStringDesc& desc)
{
	Vector size;
	bool is_size_present = false;
	Vector pos;
	bool is_pos_present = false;

	auto size_it = desc.params.find(s_GameObjectSizeFieldName);
	if (size_it != desc.params.end())
	{
		ParseString<Vector>(size_it->second, size);
		is_size_present = true;
	}

	auto pos_it = desc.params.find(s_GameObjectPositionFieldName);
	if (pos_it != desc.params.end())
	{
		ParseString<Vector>(pos_it->second, pos);
		is_pos_present = true;
	}

	if (is_size_present && is_pos_present)
	{
		return new GameObject(size, pos, PrefabCreationKey());
	}
	else if (is_size_present)
	{
		return new GameObject(size, PrefabCreationKey());
	}

	return new GameObject(PrefabCreationKey());
}

void GameObjectSerializer::DeserializeComponents(GameObject* game_object, const GameObjectStringDesc& desc)
{
	for (const ComponentStringDesc& component_desc : desc.components)
	{
		ObjectComponent* component = ComponentSerializer::DeserializeComponent(component_desc);
		if (component != nullptr)
		{
			game_object->AddComponent(component);
		}
	}
}
