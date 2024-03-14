#include "AssetExplorer.h"

#include "../FlatEngine/utility.h"
#include "../FlatEngine/GameObjectLoader.h"
#include "../FlatEngine/AssetManager.h"
#include "PrefabSaver.h"

static constexpr const char* s_RootDirectory = "Test Assets";

AssetExplorer::AssetExplorer()
	: m_FilePathBuffer("basic_bullet.prefab")
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
