#include "AssetExplorer.h"

#include "../FlatEngine/utility.h"
#include "../FlatEngine/PrefabLoader.h"
#include "PrefabSaver.h"

static constexpr const char* s_RootDirectory = "Test Assets";

AssetExplorer::AssetExplorer()
	: m_FilePathBuffer("test.prefab")
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
	ImGui::Begin("Assets");

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

	ImGui::End();
}


std::unique_ptr<EditorPrefabHandle> AssetExplorer::OpenPrefab(const std::string& prefab_path)
{
	std::unique_ptr<GameObject> prefab = PrefabLoader::LoadPrefab(prefab_path.c_str());
	if (prefab == nullptr)
	{ // Make new prefab if file not present
		FE_LOG("Creating new prefab");
		prefab = std::make_unique<GameObject>(PrefabCreationKey());
	}

	return std::make_unique<EditorPrefabHandle>(std::move(prefab), prefab_path);
}

std::string AssetExplorer::AppendPathToRootDirectory(const std::string& path)
{
	std::stringstream ss;
	ss << s_RootDirectory << '\\' << path;
	return ss.str();
}