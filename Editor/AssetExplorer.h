#pragma once
#include "../FlatEngine/imgui/imgui.h"
#include "AssetHandles.h"
#include "AssetMiner.h"

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

	void RenderMainFolders();

	void RenderCurrentFolderContent();
	void RenderDirectoryElement(const files::DirectoryElement& elem);

	void OpenDirectoryElement(const files::DirectoryElement& elem);
	void EnterDirectory(const files::Directory& dir);
	void OpenAssetFile(const files::AssetFile& file);

	void UpdateCurrentDirectoryContents();

private:
	static constexpr size_t s_FilePathSize = 256;
	char m_FilePathBuffer[s_FilePathSize];

	std::string m_CurrDirPath;
	std::vector<std::unique_ptr<files::DirectoryElement>> m_CurrDirectoryContents;
	std::vector<bool> m_CurrDirectoryContentsSelectionStates;

	IAssetListener* m_Listener;
};

