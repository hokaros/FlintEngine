#pragma once
#include <InlineGameObject.h>
#include <IHierarchyEditable.h>
#include "AssetInterfaces.h"

class EditorGameObjectHandle
	: public ISaveable
{
public:
	virtual IEditableGameObject& GetGameObject() const = 0;
	const GameObject& GetResult() const;

	// Modifying the underlying object
	void SetPosition(const Vector& pos);
	void SetSize(const Vector& size);
	void SetName(const std::string& name);

	void AddComponent(std::unique_ptr<ObjectComponent> component);
	void ModifyComponentField(std::unique_ptr<ComponentFieldChange> modification);
	void RemoveComponent(size_t component_index);

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