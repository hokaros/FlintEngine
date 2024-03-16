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

	// path - a path relative to the Assets folder
	virtual GameObject* GetPrefab(const std::string& path);
	SDL_Surface* GetSurfaceAsset(const std::string& path);

public:
	static constexpr const char* s_RootFolder = "Assets";

private:
	void AddSurfaceAsset(const char* path);
	void AddPrefab(const char* path);

	std::string GetFullPath(const char* relative_path) const;

private:
	std::map<std::string, SDL_Surface*> m_PathToSurfaceDict;
	std::map<std::string, std::unique_ptr<GameObject>> m_PathToPrefabDict;

	static AssetManager* s_Instance;
};

