#pragma once
#include <imgui/imgui.h>
#include "AssetMiner.h"

class IDirectoryOpenedObserver
{
public:
	virtual void OnDirectoryOpened(const files::Directory& dir) = 0;

	virtual ~IDirectoryOpenedObserver() = default;
};

class DirectoryTreeExplorer
{
public:
	DirectoryTreeExplorer(const std::string& root_directory);

	void RenderEmbedded();

	void SubscribeEvents(IDirectoryOpenedObserver& observer);

private:
	void RenderDirectoryNode(const files::Directory& dir);

	void NotifyDirectoryOpened(const files::Directory& dir);

private:
	const files::Directory m_RootDirectory;

	std::vector<IDirectoryOpenedObserver*> m_Observers;

	static ImGuiTreeNodeFlags s_BaseNodeFlags;
};

