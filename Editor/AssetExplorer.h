#pragma once
#include "../FlatEngine/imgui/imgui.h"
#include "AssetHandles.h"
#include "AssetMiner.h"
#include "DirectoryTreeExplorer.h"
#include "Widgets.h"

class IAssetListener
{
public:
	virtual void OnPrefabOpened(std::unique_ptr<EditorPrefabHandle> prefab) = 0;
	virtual void OnSceneOpened(std::unique_ptr<EditorSceneHandle> scene) = 0;
	
	virtual ~IAssetListener() = default;
};

class AssetExplorer
	: private IDirectoryOpenedObserver
{
public:
	AssetExplorer();

	void RegisterAssetListener(IAssetListener* listener);
	void Render();

public:
	static constexpr const char* s_RootDirectory = "Assets";

private:
	void RenderMainFolders();

	void RenderCurrentFolderContent();
	void RenderDirectoryElement(const files::DirectoryElement& elem);

	void OpenDirectoryElement(const files::DirectoryElement& elem);
	void RequestEnterDirectory(const files::Directory& dir);
	void OpenAssetFile(const files::AssetFile& file);

	void ProcessEnterDirectoryRequest();
	void EnterDirectory(const files::Directory& dir);

	void UpdateCurrentDirectoryContents();

	void AddPrefab(std::string name);
	void OpenAddAssetPrompt();
	void OpenAddScenePrompt();

	std::string GetPathToCurrDir() const;
	const files::DirectoryElement* FindElementInCurrentDirectory(const std::string& filename) const;

	// IDirectoryOpenedObserver
	virtual void OnDirectoryOpened(const files::Directory& dir) override;

private:
	std::string m_CurrDirPath;
	std::vector<std::unique_ptr<files::DirectoryElement>> m_CurrDirectoryContents;
	const files::DirectoryElement* m_CurrSelectedFile = nullptr;

	std::optional<files::Directory> m_RequestedDirectory = std::nullopt;

	DirectoryTreeExplorer m_TreeExplorer;
	ModalStringPrompt m_NewAssetNamePrompt;
	ModalChoicePrompt m_AssetTypePrompt;

	IAssetListener* m_Listener;
};

