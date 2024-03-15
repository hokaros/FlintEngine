#pragma once
#include <iostream>
#include <map>

#include "Draw.h"
#include "GameObject.h"

class AssetManager
{
public:
	AssetManager();
	static AssetManager* GetInstance();
	~AssetManager();

	virtual GameObject* GetPrefab(const std::string& path);
	void AddSurfaceAsset(const char* path);
	SDL_Surface* GetSurfaceAsset(const std::string& path);

private:
	void AddPrefab(const char* path);

private:
	std::map<std::string, SDL_Surface*> m_PathToSurfaceDict;
	std::map<std::string, std::unique_ptr<GameObject>> m_PathToPrefabDict;

	static AssetManager* s_Instance;
};

