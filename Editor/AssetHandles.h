#pragma once
#include "../FlatEngine/GameObject.h"

class EditorGameObjectHandle
{
public:
	EditorGameObjectHandle(GameObject* game_object);

	GameObject* GetGameObject() const;
	virtual void SaveGameObject();

protected:
	GameObject* m_GameObject;
};

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