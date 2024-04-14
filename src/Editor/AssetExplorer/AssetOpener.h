#pragma once
#include <AssetHandles/EditorUniversalHandle.h>
#include "AssetMiner.h"

class AssetOpener
{
public:
	static std::unique_ptr<EditorPrefabHandle> OpenPrefab(const std::string& prefab_path); // This will create a prefab if a directory exists
	static std::unique_ptr<EditorSceneHandle> OpenScene(const std::string& scene_path);

	static std::unique_ptr<EditorUniversalHandle> OpenAsset(const std::string& asset_path, files::AssetType asset_type);
};

