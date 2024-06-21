#pragma once
#include <Editables/EditableScene.h>
#include <AssetHandles/IHierarchyEditable.h>
#include <AssetHandles/ISaveable.h>

class EditorSceneHandle
	: public ISaveable
	, public IHierarchyEditable
{
public:
	EditorSceneHandle(std::unique_ptr<EditableScene> scene, const std::string& scene_path);

	EditableScene& GetScene() const;

	// ISaveable
	virtual void Save() override;
	virtual bool HasUnsavedChanges() const override;
	virtual void OnUnsavedChange() override;

	// IHierarchyEditable
	virtual const std::vector<std::unique_ptr<GameObject>>& GetSubRootObjects() const override;
	virtual void AddChild(std::unique_ptr<GameObject> child) override;
	virtual void DeleteChild(GameObject& child) override;
	virtual const char* GetName() const override;

	bool operator==(const EditorSceneHandle& other) const;

private:
	std::unique_ptr<EditableScene> m_Scene;
	std::string m_ScenePath;

	bool m_HasUnsavedChanges = false;
};