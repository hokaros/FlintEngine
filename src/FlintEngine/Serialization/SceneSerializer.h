#pragma once
#include "GameObjectSerializer.h"
#include <Scene/Scene.h>

struct NavmeshGenerationParamsStringDesc
{
	std::string merge_distance;
};

struct SceneStringDesc
{
	std::string background_color;
	NavmeshGenerationParamsStringDesc navmesh_generation_params;
	std::vector<std::unique_ptr<GameObjectStringDescProxy>> game_objects;
};

class SceneSerializer
{
public:
	static std::unique_ptr<SceneStringDesc> Serialize(const Scene& scene);
	static std::unique_ptr<Scene> Deserialize(const SceneStringDesc& scene_desc);

private:
	static void SerializeGameObjects(const Scene& scene, SceneStringDesc& scene_serialized);
	static void SerializeSceneParams(const Scene& scene, SceneStringDesc& scene_serialized);
	static void SerializeNavmeshGenerationParams(const Navigation::NavmeshGenerationParams& params, NavmeshGenerationParamsStringDesc& params_serialized);

	static void DeserializeGameObjects(const SceneStringDesc& scene_serialized, Scene& scene);
	static void DeserializeSceneParams(const SceneStringDesc& scene_serialized, Scene& scene);
	static void DeserializeNavmeshGenerationParams(const NavmeshGenerationParamsStringDesc& params_serialized, Navigation::NavmeshGenerationParams& params);
};

