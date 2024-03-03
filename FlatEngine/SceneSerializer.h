#pragma once
#include "GameObjectSerializer.h"
#include "Scene.h"

struct SceneStringDesc
{
	std::string background_color;
	std::vector<std::unique_ptr<GameObjectStringDesc>> game_objects;
};

class SceneSerializer
{
public:
	static std::unique_ptr<SceneStringDesc> Serialize(const Scene& scene);
	static std::unique_ptr<Scene> Deserialize(const SceneStringDesc& scene_desc);

private:
	static void SerializeGameObjects(const Scene& scene, SceneStringDesc& scene_serialized);
	static void SerializeSceneParams(const Scene& scene, SceneStringDesc& scene_serialized);

	static void DeserializeGameObjects(const SceneStringDesc& scene_serialized, Scene& scene);
	static void DeserializeSceneParams(const SceneStringDesc& scene_serialized, Scene& scene);
};

