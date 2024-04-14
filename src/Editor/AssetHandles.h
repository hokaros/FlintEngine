#pragma once
#include <InlineGameObject.h>
#include <EditableScene.h>
#include <IHierarchyEditable.h>

class ISaveable
{
public:
	virtual void Save() = 0;

	virtual bool HasUnsavedChanges() const = 0;
	virtual void OnUnsavedChange() = 0;

	virtual ~ISaveable() = default;
};

class EditorGameObjectHandle
	: public ISaveable
{
public:
	virtual IEditableGameObject& GetGameObject() const = 0;

	bool operator==(const EditorGameObjectHandle& other) const;
};

class EditorPrefabHandle
	: public EditorGameObjectHandle
{
public:
	EditorPrefabHandle(std::unique_ptr<InlineGameObject> prefab, const std::string& prefab_path);

	virtual IEditableGameObject& GetGameObject() const override;

	// ISaveable
	virtual void Save() override;
	virtual bool HasUnsavedChanges() const override;
	virtual void OnUnsavedChange() override;

private:
	std::unique_ptr<InlineGameObject> m_Prefab;
	std::string m_PrefabPath;

	bool m_HasUnsavedChanges = false;
};

class EditorIEditableGameObjectHandle
	: public EditorGameObjectHandle
{
public:
	EditorIEditableGameObjectHandle(IEditableGameObject& game_object, ISaveable& root_asset);

	virtual IEditableGameObject& GetGameObject() const override;

	// ISaveable
	virtual void Save() override;
	virtual bool HasUnsavedChanges() const override;
	virtual void OnUnsavedChange() override;

private:
	IEditableGameObject& m_EditableObject;
	ISaveable& m_RootAsset;
};


class EditorSceneHandle
	: public ISaveable
{
public:
	EditorSceneHandle(std::unique_ptr<EditableScene> scene, const std::string& scene_path);

	EditableScene& GetScene() const;

	// ISaveable
	virtual void Save() override;
	virtual bool HasUnsavedChanges() const override;
	virtual void OnUnsavedChange() override;

	bool operator==(const EditorSceneHandle& other) const;

private:
	std::unique_ptr<EditableScene> m_Scene;
	std::string m_ScenePath;

	bool m_HasUnsavedChanges = false;
};



class EditorUniversalHandle
	: public ISaveable
{
public:
	EditorUniversalHandle(std::shared_ptr<EditorGameObjectHandle> game_object);
	EditorUniversalHandle(std::shared_ptr<EditorIEditableGameObjectHandle> game_object);
	EditorUniversalHandle(std::shared_ptr<EditorSceneHandle> scene);

	IHierarchyEditable* GetHierarchyEditable() const;
	EditorGameObjectHandle* GetGameObjectHandle() const;
	EditorSceneHandle* GetSceneHandle() const;

	// ISaveable
	virtual void Save() override;
	virtual bool HasUnsavedChanges() const override;
	virtual void OnUnsavedChange() override;

	bool operator==(const EditorUniversalHandle& other) const;

private:
	bool AreGameObjectHandlesSame(const EditorUniversalHandle& other) const;
	bool AreSceneHandlesSame(const EditorUniversalHandle& other) const;
	template<typename T>
	static bool ArePointedObjectsEqual(const T* p1, const T* p2);

private:
	IHierarchyEditable* m_HierarchyEditable = nullptr;
	ISaveable* m_Saveable = nullptr;
	std::shared_ptr<EditorGameObjectHandle> m_EditableGameObject = nullptr;
	std::shared_ptr<EditorSceneHandle> m_SceneHandle = nullptr;
};

template<typename T>
inline bool EditorUniversalHandle::ArePointedObjectsEqual(const T* p1, const T* p2)
{
	if (p1 == nullptr && p2 == nullptr)
		return true;

	if (p1 == nullptr || p2 == nullptr)
		return false;

	return *p1 == *p2;
}
