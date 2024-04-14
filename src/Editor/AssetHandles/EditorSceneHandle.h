#pragma once
#include <EditableScene.h>
#include <AssetHandles/AssetInterfaces.h>

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