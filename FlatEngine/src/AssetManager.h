#pragma once
#include <iostream>
#include <map>

#include "Draw.h"
#include <Core/GameObject.h>

class AssetManager
{
public:
	AssetManager();
	static AssetManager* GetInstance();
	~AssetManager();

	// path - a path relative to the Assets folder
	virtual GameObject* GetPrefab(const std::string& path);
	SDL_Surface* GetSurfaceAsset(const std::string& path);

	void InvalidatePrefab(const std::string& path); // Invalidates cached prefab and forces reload on next query

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

