#pragma once
#include "../FlatEngine/GameObjectSerializer.h"
#include "GameObjectEditor.h"



class PrefabSaver
{
public:
	static void SavePrefab(const GameObject& prefab, const char* file_path);

private:
	PrefabSaver(std::fstream& prefab_file);

	void SavePrefab(const GameObject& prefab);
	void SaveComponents(const GameObject& prefab);
	void SaveFields(const ObjectComponent& component);
	void SaveKeyValuePairs(const std::map<std::string, std::string>& dict);

private:
	std::fstream& m_PrefabFile;
};

