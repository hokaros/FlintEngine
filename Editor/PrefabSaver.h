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
	void SaveComponents(const std::vector<std::unique_ptr<ComponentStringDesc>>& components);
	void SaveFields(const std::map<std::string, std::string>& fields);
	void SaveKeyValuePairs(const std::map<std::string, std::string>& dict, size_t indent = 0);

private:
	std::fstream& m_PrefabFile;
};

