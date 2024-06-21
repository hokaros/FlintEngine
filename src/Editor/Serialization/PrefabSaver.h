#pragma once
#include <Serialization/GameObjectSerializer.h>
#include <Serialization/PrefabInstanceSerializer.h>
#include "IndentPrinter.h"

class PrefabSaver
{
public:
	static void SavePrefab(const GameObject& prefab, const char* file_path);

	PrefabSaver(std::fstream& prefab_file, size_t start_indent);
	PrefabSaver(const IndentPrinter& indent_printer);
	void SavePrefab(const GameObject& prefab);
	void SaveGameObject(const GameObjectStringDescProxy& game_object);

public:
	void SaveInlineGameObject(const GameObjectStringDesc& game_object);
	void SavePrefabInstance(const PrefabInstanceStringDesc& prefab_instance);

	void SaveComponents(const std::vector<std::unique_ptr<ComponentStringDesc>>& components);
	void SaveChildren(const std::vector< std::unique_ptr<GameObjectStringDescProxy>>& children);

private:
	IndentPrinter m_IndentPrinter;
};

