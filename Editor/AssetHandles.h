#pragma once
#include "../FlatEngine/InlineGameObject.h"
#include "../FlatEngine/EditableScene.h"
#include "../FlatEngine/IHierarchyEditable.h"

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


class EditorSceneHandle
{
public:
	EditorSceneHandle(std::unique_ptr<EditableScene> scene, const std::string& scene_path);

	EditableScene* GetScene() const;
	void SaveScene();

private:
	std::unique_ptr<EditableScene> m_Scene;
	std::string m_ScenePath;

};



class EditorUniversalHandle
{
public:
	EditorUniversalHandle(std::shared_ptr<EditorGameObjectHandle> game_object);
	EditorUniversalHandle(std::shared_ptr<EditorPrefabHandle> prefab);
	EditorUniversalHandle(std::shared_ptr<EditorSceneHandle> scene);

	IHierarchyEditable* GetHierarchyEditable() const;
	std::shared_ptr<EditorGameObjectHandle> GetGameObjectHandle() const;
	std::shared_ptr<EditorSceneHandle> GetSceneHandle() const;

private:
	IHierarchyEditable* m_HierarchyEditable = nullptr;
	std::shared_ptr<EditorGameObjectHandle> m_EditableGameObject = nullptr;
	std::shared_ptr<EditorSceneHandle> m_SceneHandle = nullptr;
};