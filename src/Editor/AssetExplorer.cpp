#include "AssetExplorer.h"

#include <utility.h>
#include <AssetManager.h>
#include "AssetOpener.h"

AssetExplorer::AssetExplorer()
	: m_CurrDirPath(s_RootDirectory)
	, m_TreeExplorer(s_RootDirectory)
	, m_NewAssetNamePrompt("Add asset", "Asset name")
	, m_AssetTypePrompt("Select asset type")
{
	files::AssetMiner::CreateDirectory(s_RootDirectory);
	UpdateCurrentDirectoryContents();

	m_TreeExplorer.SubscribeEvents(*this);

	m_AssetTypePrompt.AddChoice("Prefab", [this]() {
		OpenAddPrefabPrompt();
	});
	m_AssetTypePrompt.AddChoice("Scene", [this]() {
		OpenAddScenePrompt();
	});
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

void AssetExplorer::RenderMainFolders()
{
	ImVec2 panel_size = ImVec2(ImGui::GetContentRegionAvail().x * 0.2f, ImGui::GetContentRegionAvail().y);
	if (ImGui::BeginChild("Main folders", panel_size, true))
	{
		m_TreeExplorer.RenderEmbedded();
	}
	ImGui::EndChild();
}

void AssetExplorer::RenderCurrentFolderContent()
{
	ProcessEnterDirectoryRequest();

	if (ImGui::BeginChild("Current folder"))
	{
		ImGui::Text("Contents of %s:", m_CurrDirPath.c_str());
		if (ImGui::Button("+"))
		{
			m_AssetTypePrompt.Open();
		}

		m_AssetTypePrompt.Update();
		m_NewAssetNamePrompt.Update();

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
	const bool is_selected = m_CurrSelectedFile != nullptr && m_CurrSelectedFile == &elem;

	if (ImGui::Selectable(elem.GetFileName().c_str(), is_selected, ImGuiSelectableFlags_AllowDoubleClick))
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
		RequestEnterDirectory(files::Directory::SpecificCast(elem));
	}
	else
	{
		OpenAssetFile(files::AssetFile::SpecificCast(elem));
	}
}

void AssetExplorer::RequestEnterDirectory(const files::Directory& dir)
{
	m_RequestedDirectory = dir;
}

void AssetExplorer::ProcessEnterDirectoryRequest()
{
	if (m_RequestedDirectory.has_value())
	{
		EnterDirectory(m_RequestedDirectory.value());
	}

	m_RequestedDirectory.reset();
}

void AssetExplorer::EnterDirectory(const files::Directory& dir)
{
	m_CurrDirPath = dir.GetPath();
	UpdateCurrentDirectoryContents();
}

void AssetExplorer::OpenAssetFile(const files::AssetFile& file)
{
	m_CurrSelectedFile = &file;

	if (m_Listener == nullptr)
	{
		FE_WARN("No asset listener");
		return;
	}

	switch (file.GetAssetType())
	{
	case files::AssetType::Prefab:
		m_Listener->OnPrefabOpened(AssetOpener::OpenPrefab(file.GetPath()));
		break;
	case files::AssetType::Scene:
		m_Listener->OnSceneOpened(AssetOpener::OpenScene(file.GetPath()));
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

	m_CurrSelectedFile = nullptr;
}

void AssetExplorer::CreateAsset(const std::string& name, files::AssetType asset_type)
{
	if (name.empty())
		return;

	std::string filename = name + files::AssetFile::GetAssetExtension(asset_type);
	// TODO: find if filename exists

	std::string full_path = m_CurrDirPath + "\\" + filename;
	std::unique_ptr<EditorUniversalHandle> asset_handle = AssetOpener::OpenAsset(full_path, asset_type);
	asset_handle->Save();

	UpdateCurrentDirectoryContents();

	const files::DirectoryElement* new_file = FindElementInCurrentDirectory(filename);
	if (new_file == nullptr)
	{
		FE_LOG("Cannot find new file");
	}
	else
	{
		OpenAssetFile(files::AssetFile::SpecificCast(*new_file));
	}
}

void AssetExplorer::OpenAddAssetPrompt(files::AssetType asset_type)
{
	m_NewAssetNamePrompt.SetAcceptCallback("Add", [this, asset_type](std::string name) {
		CreateAsset(name, asset_type);
		});
	m_NewAssetNamePrompt.Open();
}

void AssetExplorer::OpenAddPrefabPrompt()
{
	m_NewAssetNamePrompt.SetLabel("Prefab name");
	OpenAddAssetPrompt(files::AssetType::Prefab);
}

void AssetExplorer::OpenAddScenePrompt()
{
	m_NewAssetNamePrompt.SetLabel("Scene name");
	OpenAddAssetPrompt(files::AssetType::Scene);
}

std::string AssetExplorer::GetPathToCurrDir() const
{
	return m_CurrDirPath;
}

const files::DirectoryElement* AssetExplorer::FindElementInCurrentDirectory(const std::string& filename) const
{
	for (const std::unique_ptr<files::DirectoryElement>& dir_elem : m_CurrDirectoryContents)
	{
		if (dir_elem->GetFileName() == filename)
		{
			return dir_elem.get();
		}
	}

	return nullptr;
}

void AssetExplorer::OnDirectoryOpened(const files::Directory& dir)
{
	RequestEnterDirectory(dir);
}