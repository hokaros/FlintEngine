#include "AssetManager.h"

#include "utility.h"

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
	for (auto& entry : m_PathToAssetDict)
	{
		SDL_Surface* asset = entry.second;
		SDL_FreeSurface(asset);
	}

	if (s_Instance == this)
	{
		s_Instance = nullptr;
	}
}

void AssetManager::AddAsset(const char* path)
{
	SDL_Surface* asset = SDL_LoadBMP(path);
	if (asset == nullptr)
	{
		FE_ASSERT(false, "Cannot load asset: %s", path);
		return;
	}

	if (m_PathToAssetDict.find(std::string(path)) != m_PathToAssetDict.end())
	{
		FE_LOG("Asset already added: %s", path);
		SDL_FreeSurface(asset);
		return;
	}

	m_PathToAssetDict.insert({ std::string(path), asset });
}

SDL_Surface* AssetManager::GetSurfaceAsset(const std::string& path)
{
	auto it = m_PathToAssetDict.find(path);
	if (it == m_PathToAssetDict.end())
	{
		// Add asset anyway
		// TODO: potentially insecure
		AddAsset(path.c_str());

		it = m_PathToAssetDict.find(path);
		if (it == m_PathToAssetDict.end())
		{
			return nullptr;
		}
	}

	return it->second;
}
