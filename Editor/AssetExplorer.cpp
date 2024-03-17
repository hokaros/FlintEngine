#include "AssetExplorer.h"

#include "../FlatEngine/utility.h"
#include "../FlatEngine/GameObjectLoader.h"
#include "../FlatEngine/AssetManager.h"
#include "PrefabSaver.h"

static constexpr const char* s_RootDirectory = "Assets";

AssetExplorer::AssetExplorer()
	: m_FilePathBuffer("lab.prefab")
	, m_CurrDirPath(s_RootDirectory)
{

}

void AssetExplorer::RegisterAssetListener(IAssetListener* listener)
{
	if (m_Listener != nullptr)
	{
		FE_WARN("Overriding previous listener");
	}

	m_Listener = listener;
}

void AssetExplorer::Render()
{
	if (ImGui::Begin("Assets"))
	{
		RenderMainFolders();
		ImGui::SameLine();
		RenderCurrentFolderContent();
	}
	ImGui::End();
}

GameObject* AssetExplorer::GetPrefab(const std::string& path)
{
	std::string full_path = AppendPathToRootDirectory(path);
	return AssetManager::GetInstance()->GetPrefab(full_path);
}


std::unique_ptr<EditorPrefabHandle> AssetExplorer::OpenPrefab(const std::string& prefab_path)
{
	std::unique_ptr<InlineGameObject> prefab = GameObjectLoader::LoadPrefab(prefab_path.c_str());
	if (prefab == nullptr)
	{ // Make new prefab if file not present
		FE_LOG("Creating new prefab");
		prefab = std::make_unique<InlineGameObject>();
	}

	return std::make_unique<EditorPrefabHandle>(std::move(prefab), prefab_path);
}

std::string AssetExplorer::AppendPathToRootDirectory(const std::string& path)
{
	std::stringstream ss;
	ss << s_RootDirectory << '\\' << path;
	return ss.str();
}

void AssetExplorer::RenderMainFolders()
{
	ImVec2 panel_size = ImVec2(ImGui::GetContentRegionAvail().x * 0.2f, ImGui::GetContentRegionAvail().y);
	if (ImGui::BeginChild("Main folders", panel_size, true))
	{
		ImGui::Text("Main");
	}
	ImGui::EndChild();
}

void AssetExplorer::RenderCurrentFolderContent()
{
	if (ImGui::BeginChild("Current folder"))
	{
		UpdateCurrentDirectoryContents(); // TODO: do this only when navigating into a directory OR adding/removing new elements

		ImGui::Text("Contents of %s:", m_CurrDirPath.c_str());

		for (const std::unique_ptr<files::DirectoryElement>& elem : m_CurrDirectoryContents)
		{
			if (elem->GetType() == files::DirectoryElement::Type::Directory)
			{
				ImGui::BulletText("[Dir] %s", elem->GetFileName().c_str());
			}
			else
			{
				ImGui::BulletText("%s", elem->GetFileName().c_str());
			}
		}

		// Temporary opening
		ImGui::InputText("File path", m_FilePathBuffer, s_FilePathSize);

		if (ImGui::Button("Open"))
		{
			if (m_Listener != nullptr)
			{
				std::string full_path = AppendPathToRootDirectory(m_FilePathBuffer);
				m_Listener->OnPrefabOpened(OpenPrefab(full_path));
			}
			else
			{
				FE_WARN("No asset listener");
			}
		}
	}
	ImGui::EndChild();
}

void AssetExplorer::UpdateCurrentDirectoryContents()
{
	files::Directory dir = files::Directory(m_CurrDirPath);

	m_CurrDirectoryContents.clear();
	files::AssetMiner::GetDirectoryContents(dir, m_CurrDirectoryContents);
}
