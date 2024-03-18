#include "AssetOpener.h"
#include "../FlatEngine/GameObjectLoader.h"
#include "../FlatEngine/SceneLoader.h"


std::unique_ptr<EditorPrefabHandle> AssetOpener::OpenPrefab(const std::string& prefab_path)
{
	std::unique_ptr<InlineGameObject> prefab = GameObjectLoader::LoadPrefab(prefab_path.c_str());
	if (prefab == nullptr)
	{ // Make new prefab if file not present
		FE_LOG("Creating new prefab");
		prefab = std::make_unique<InlineGameObject>();
	}

	return std::make_unique<EditorPrefabHandle>(std::move(prefab), prefab_path);
}

std::unique_ptr<EditorSceneHandle> AssetOpener::OpenScene(const std::string& scene_path)
{
	std::unique_ptr<EditableScene> scene = SceneLoader::LoadScene(scene_path.c_str());
	if (scene == nullptr)
	{
		FE_LOG("Creating new scene");
		scene = std::make_unique<EditableScene>();
	}

	return std::make_unique<EditorSceneHandle>(std::move(scene), scene_path);
}
