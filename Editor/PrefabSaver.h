#pragma once
#include "../FlatEngine/GameObjectSerializer.h"
#include "GameObjectEditor.h"
#include "IndentPrinter.h"

class PrefabSaver
{
public:
	static void SavePrefab(const GameObject& prefab, const char* file_path);

private:
	PrefabSaver(std::fstream& prefab_file, size_t start_indent);

	void SavePrefab(const GameObject& prefab);

	void SaveGameObject(const GameObjectStringDesc& game_object);
	void SaveComponents(const std::vector<std::unique_ptr<ComponentStringDesc>>& components);
	void SaveChildren(const std::vector< std::unique_ptr<GameObjectStringDesc>>& children);

	void SaveKeyValuePairs(const std::map<std::string, std::string>& dict);

private:
	IndentPrinter m_IndentPrinter;
};

