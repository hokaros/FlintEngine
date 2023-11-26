#pragma once
#include "../FlatEngine/GameObjectSerializer.h"
#include "GameObjectEditor.h"


class EditorPrefabHandle
	: public EditorGameObjectHandle
{
public:
	EditorPrefabHandle(std::unique_ptr<GameObject> prefab, const std::string& prefab_path);

	virtual void SaveGameObject() override;

private:
	std::unique_ptr<GameObject> m_Prefab;
	std::string m_PrefabPath;
};


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

