#include "AssetManager.h"

#include "utility.h"
#include "PrefabLoader.h"

AssetManager* AssetManager::s_Instance = nullptr;

AssetManager::AssetManager()
{
	s_Instance = this;
}

AssetManager* AssetManager::GetInstance()
{
	return s_Instance;
}

AssetManager::~AssetManager()
{
	for (auto& entry : m_PathToSurfaceDict)
	{
		SDL_Surface* asset = entry.second;
		SDL_FreeSurface(asset);
	}

	if (s_Instance == this)
	{
		s_Instance = nullptr;
	}
}

void AssetManager::AddSurfaceAsset(const char* path)
{
	SDL_Surface* asset = SDL_LoadBMP(path);
	if (asset == nullptr)
	{
		FE_ASSERT(false, "Cannot load surface: %s", path);
		return;
	}

	if (m_PathToSurfaceDict.find(std::string(path)) != m_PathToSurfaceDict.end())
	{
		FE_LOG("Surface already added: %s", path);
		SDL_FreeSurface(asset);
		return;
	}

	m_PathToSurfaceDict.insert({ std::string(path), asset });
}

void AssetManager::AddPrefab(const char* path)
{
	std::unique_ptr<GameObject> prefab = PrefabLoader::LoadPrefab(path);
	if (prefab == nullptr)
	{
		FE_ASSERT(false, "Cannot load prefab: %s", path);
		return;
	}

	if (m_PathToPrefabDict.find(std::string(path)) != m_PathToPrefabDict.end())
	{
		FE_LOG("Prefab already added: %s", path);
		return;
	}

	m_PathToPrefabDict.insert({ std::string(path), std::move(prefab) });
}

GameObject* AssetManager::GetPrefab(const std::string& path)
{
	auto it = m_PathToPrefabDict.find(path);
	if (it == m_PathToPrefabDict.end())
	{
		AddPrefab(path.c_str());

		it = m_PathToPrefabDict.find(path);
		if (it == m_PathToPrefabDict.end())
		{
			return nullptr;
		}
	}

	return it->second.get();
}

SDL_Surface* AssetManager::GetSurfaceAsset(const std::string& path)
{
	auto it = m_PathToSurfaceDict.find(path);
	if (it == m_PathToSurfaceDict.end())
	{
		AddSurfaceAsset(path.c_str());

		it = m_PathToSurfaceDict.find(path);
		if (it == m_PathToSurfaceDict.end())
		{
			return nullptr;
		}
	}

	return it->second;
}
