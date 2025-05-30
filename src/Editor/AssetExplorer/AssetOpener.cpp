#include "AssetOpener.h"
#include <Serialization/GameObjectLoader.h>
#include <Serialization/SceneLoader.h>
#include "AssetMiner.h"


std::unique_ptr<EditorPrefabHandle> AssetOpener::OpenPrefab(const std::string& prefab_path)
{
	std::unique_ptr<GameObject> prefab = GameObjectLoader::LoadPrefab(prefab_path.c_str());
	if (prefab == nullptr)
	{ // Make new prefab if file not present
		FE_LOG("Creating new prefab");
		prefab = std::make_unique<GameObject>();
	}

	return std::make_unique<EditorPrefabHandle>(std::move(prefab), prefab_path);
}

std::unique_ptr<EditorSceneHandle> AssetOpener::OpenScene(const std::string& scene_path)
{
	std::unique_ptr<Scene> scene = SceneLoader::LoadScene(scene_path.c_str());
	if (scene == nullptr)
	{
		FE_LOG("Creating new scene");
		scene = std::make_unique<Scene>();
	}

	return std::make_unique<EditorSceneHandle>(std::move(scene), scene_path);
}

std::unique_ptr<EditorUniversalHandle> AssetOpener::OpenAsset(const std::string& asset_path, files::AssetType asset_type)
{
	switch (asset_type)
	{
	case files::AssetType::Prefab:
		return std::make_unique<EditorUniversalHandle>(OpenPrefab(asset_path));

	case files::AssetType::Scene:
		return std::make_unique<EditorUniversalHandle>(OpenScene(asset_path));

	case files::AssetType::Unknown:
		FE_ASSERT(false, "Cannot open asset if no AssetType passed");
		return nullptr;

	default:
		FE_ASSERT(false, "Unimplemented asset opening");
		return nullptr;
	}
}
