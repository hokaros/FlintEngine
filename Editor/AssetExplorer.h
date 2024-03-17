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
	void RequestEnterDirectory(const files::Directory& dir);
	void OpenAssetFile(const files::AssetFile& file);

	void ProcessEnterDirectoryRequest();
	void EnterDirectory(const files::Directory& dir);

	void UpdateCurrentDirectoryContents();

private:
	std::string m_CurrDirPath;
	std::vector<std::unique_ptr<files::DirectoryElement>> m_CurrDirectoryContents;
	const files::DirectoryElement* m_CurrSelectedFile = nullptr;

	const files::Directory* m_RequestedDirectory = nullptr;

	IAssetListener* m_Listener;
};

