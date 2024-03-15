#pragma once
#include "../FlatEngine/GameObjectSerializer.h"
#include "../FlatEngine/PrefabInstanceSerializer.h"
#include "GameObjectEditor.h"
#include "IndentPrinter.h"

class PrefabSaver
{
public:
	static void SavePrefab(const InlineGameObject& prefab, const char* file_path);

private:
	PrefabSaver(std::fstream& prefab_file, size_t start_indent);

	void SavePrefab(const InlineGameObject& prefab);

	void SaveGameObject(const GameObjectStringDescProxy& game_object);
	void SaveInlineGameObject(const GameObjectStringDesc& game_object);
	void SavePrefabInstance(const PrefabInstanceStringDesc& prefab_instance);

	void SaveComponents(const std::vector<std::unique_ptr<ComponentStringDesc>>& components);
	void SaveChildren(const std::vector< std::unique_ptr<GameObjectStringDescProxy>>& children);

	void SaveKeyValuePair(const std::string& key, const std::string& value);
	void SaveKeyValuePairs(const std::map<std::string, std::string>& dict);

private:
	IndentPrinter m_IndentPrinter;
};

