#pragma once
#include <AssetHandles/EditorGameObjectHandle.h>
#include <AssetHandles/EditorSceneHandle.h>


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
