#pragma once
#include "GameObjectSerializer.h"
#include <Editables/EditableScene.h>

struct SceneStringDesc
{
	std::string background_color;
	std::vector<std::unique_ptr<GameObjectStringDescProxy>> game_objects;
};

class SceneSerializer
{
public:
	static std::unique_ptr<SceneStringDesc> Serialize(const EditableScene& scene);
	static std::unique_ptr<EditableScene> Deserialize(const SceneStringDesc& scene_desc);

private:
	static void SerializeGameObjects(const EditableScene& scene, SceneStringDesc& scene_serialized);
	static void SerializeSceneParams(const EditableScene& scene, SceneStringDesc& scene_serialized);

	static void DeserializeGameObjects(const SceneStringDesc& scene_serialized, EditableScene& scene);
	static void DeserializeSceneParams(const SceneStringDesc& scene_serialized, EditableScene& scene);
};

