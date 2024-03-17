#include "DirectoryTreeExplorer.h"

ImGuiTreeNodeFlags DirectoryTreeExplorer::s_BaseNodeFlags = ImGuiTreeNodeFlags_OpenOnArrow;

DirectoryTreeExplorer::DirectoryTreeExplorer(const std::string& root_directory)
	: m_RootDirectory(root_directory)
{
}

void DirectoryTreeExplorer::RenderEmbedded()
{
	RenderDirectoryNode(m_RootDirectory);
}

void DirectoryTreeExplorer::SubscribeEvents(IDirectoryOpenedObserver& observer)
{
	m_Observers.push_back(&observer);
}

void DirectoryTreeExplorer::RenderDirectoryNode(const files::Directory& dir)
{
	std::string dir_name = dir.GetFileName();
	const bool node_open = ImGui::TreeNodeEx(dir_name.c_str(), s_BaseNodeFlags);
	if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
	{
		NotifyDirectoryOpened(dir);
	}

	if (node_open)
	{
		std::vector<files::Directory> child_dirs;
		files::AssetMiner::GetDirectoriesInside(dir, child_dirs); // TODO: cache this

		for (const files::Directory& child_dir : child_dirs)
		{
			RenderDirectoryNode(child_dir);
		}

		ImGui::TreePop();
	}
}

void DirectoryTreeExplorer::NotifyDirectoryOpened(const files::Directory& dir)
{
	for (IDirectoryOpenedObserver* observer : m_Observers)
	{
		observer->OnDirectoryOpened(dir);
	}
}
