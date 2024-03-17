#pragma once
#include "AssetHandles.h"

class AssetOpener
{
public:
	static std::unique_ptr<EditorPrefabHandle> OpenPrefab(const std::string& prefab_path); // This will create a prefab if a directory exists
};

