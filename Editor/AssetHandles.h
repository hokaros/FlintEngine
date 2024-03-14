#pragma once
#include "../FlatEngine/InlineGameObject.h"

class EditorGameObjectHandle
{
public:
	virtual IEditableGameObject* GetGameObject() const = 0;
	virtual void SaveInlineGameObject() = 0;
};

class EditorPrefabHandle
	: public EditorGameObjectHandle
{
public:
	EditorPrefabHandle(std::unique_ptr<InlineGameObject> prefab, const std::string& prefab_path);

	virtual IEditableGameObject* GetGameObject() const override;
	virtual void SaveInlineGameObject() override;

private:
	std::unique_ptr<InlineGameObject> m_Prefab;
	std::string m_PrefabPath;
};

class EditorIEditableGameObjectHandle
	: public EditorGameObjectHandle
{
public:
	EditorIEditableGameObjectHandle(IEditableGameObject* game_object);

	virtual IEditableGameObject* GetGameObject() const override;
	virtual void SaveInlineGameObject() override;

private:
	IEditableGameObject* m_EditableObject;
};