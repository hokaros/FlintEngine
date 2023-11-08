#pragma once
#include <iostream>
#include <map>

#include "Draw.h"

class AssetManager
{
public:
	AssetManager();
	static AssetManager* GetInstance();
	~AssetManager();

	void AddAsset(const char* path);
	SDL_Surface* GetSurfaceAsset(const std::string& path);

private:
	std::map<std::string, SDL_Surface*> m_PathToAssetDict;

	static AssetManager* s_Instance;
};

