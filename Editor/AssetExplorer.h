#pragma once
#include "../FlatEngine/imgui/imgui.h"
#include "AssetHandles.h"

class IAssetListener
{
public:
	virtual void OnPrefabOpened(std::unique_ptr<EditorPrefabHandle> prefab) = 0;
	
	virtual ~IAssetListener() = default;
};

class AssetExplorer
{
public:
	AssetExplorer();

	void RegisterAssetListener(IAssetListener* listener);
	void Render();

private:
	std::unique_ptr<EditorPrefabHandle> OpenPrefab(const std::string& prefab_path); // This will create a prefab if a directory exists
	std::string AppendPathToRootDirectory(const std::string& path);

private:
	static constexpr size_t s_FilePathSize = 256;
	char m_FilePathBuffer[s_FilePathSize];

	IAssetListener* m_Listener;
};
