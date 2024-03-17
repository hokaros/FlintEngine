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
		UpdateCurrentDirectoryContents(); // TODO: do this only when navigating into a directory OR adding/removing new elements

		RenderMainFolders();
		ImGui::SameLine();
		RenderCurrentFolderContent();
	}
	ImGui::End();
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
		ImGui::Text("Contents of %s:", m_CurrDirPath.c_str());

		if (ImGui::BeginChild("Current folder contents"))
		{
			for (const std::unique_ptr<files::DirectoryElement>& elem : m_CurrDirectoryContents)
			{
				RenderDirectoryElement(*elem);
			}
		}
		ImGui::EndChild();
	}
	ImGui::EndChild();
}

void AssetExplorer::RenderDirectoryElement(const files::DirectoryElement& elem)
{
	if (ImGui::Selectable(elem.GetFileName().c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
	{
		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			OpenDirectoryElement(elem);
		}
	}
}

void AssetExplorer::OpenDirectoryElement(const files::DirectoryElement& elem)
{
	if (elem.GetType() == files::DirectoryElement::Type::Directory)
	{
		EnterDirectory(files::Directory::SpecificCast(elem));
	}
	else
	{
		OpenAssetFile(files::AssetFile::SpecificCast(elem));
	}
}

void AssetExplorer::EnterDirectory(const files::Directory& dir)
{
	// TODO
	std::cout << "Trying to enter: " << dir.GetPath() << std::endl;
}

void AssetExplorer::OpenAssetFile(const files::AssetFile& file)
{
	if (m_Listener == nullptr)
	{
		FE_WARN("No asset listener");
		return;
	}

	switch (file.GetAssetType())
	{
	case files::AssetType::Prefab:
		m_Listener->OnPrefabOpened(OpenPrefab(file.GetPath()));
		break;
	default:
		FE_LOG("Unimplemented asset opening");
	}
}

void AssetExplorer::UpdateCurrentDirectoryContents()
{
	files::Directory dir = files::Directory(m_CurrDirPath);

	m_CurrDirectoryContents.clear();
	files::AssetMiner::GetDirectoryContents(dir, m_CurrDirectoryContents);
}
