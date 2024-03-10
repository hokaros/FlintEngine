#pragma once
#include "../FlatEngine/PlainGameObject.h"

class EditorGameObjectHandle
{
public:
	virtual IEditableGameObject* GetGameObject() const = 0;
	virtual void SaveGameObject() = 0;
};

class EditorPrefabHandle
	: public EditorGameObjectHandle
{
public:
	EditorPrefabHandle(std::unique_ptr<GameObject> prefab, const std::string& prefab_path);

	virtual IEditableGameObject* GetGameObject() const override;
	virtual void SaveGameObject() override;

private:
	std::unique_ptr<GameObject> m_Prefab;
	std::unique_ptr<PlainGameObject> m_EditablePrefab;
	std::string m_PrefabPath;
};

class EditorIEditableGameObjectHandle
	: public EditorGameObjectHandle
{
public:
	EditorIEditableGameObjectHandle(IEditableGameObject* game_object);

	virtual IEditableGameObject* GetGameObject() const override;
	virtual void SaveGameObject() override;

private:
	IEditableGameObject* m_EditableObject;
};

class EditorPlainGameObjectHandle
	: public EditorGameObjectHandle
{
public:
	EditorPlainGameObjectHandle(GameObject* game_object);

	virtual IEditableGameObject* GetGameObject() const override;
	virtual void SaveGameObject() override;

private:
	std::unique_ptr<IEditableGameObject> m_EditableObject;
};