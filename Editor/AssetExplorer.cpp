#include "AssetExplorer.h"

#include "../FlatEngine/utility.h"
#include "../FlatEngine/GameObjectLoader.h"
#include "../FlatEngine/AssetManager.h"
#include "PrefabSaver.h"
#include "AssetOpener.h"

AssetExplorer::AssetExplorer()
	: m_CurrDirPath(s_RootDirectory)
	, m_TreeExplorer(s_RootDirectory)
	, m_AddPrefabPrompt("Add prefab", "Prefab name")
{
	UpdateCurrentDirectoryContents();

	m_TreeExplorer.SubscribeEvents(*this);
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
			m_AddPrefabPrompt.Open();
		}
		UpdateAddFilePrompt();

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

void AssetExplorer::UpdateAddFilePrompt()
{
	std::string filename;
	if (m_AddPrefabPrompt.Update(filename) && !filename.empty())
	{
		std::string filename_extended = filename + ".prefab";
		// TODO: find if filename exists

		std::string full_path = m_CurrDirPath + "\\" + filename_extended;
		std::unique_ptr<EditorPrefabHandle> prefab_handle = AssetOpener::OpenPrefab(full_path);
		prefab_handle->SaveInlineGameObject();
		UpdateCurrentDirectoryContents();

		const files::DirectoryElement* new_file = FindElementInCurrentDirectory(filename_extended);
		if (new_file == nullptr)
		{
			FE_LOG("Cannot find new file");
		}
		else
		{
			OpenAssetFile(files::AssetFile::SpecificCast(*new_file));
		}
	}
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
