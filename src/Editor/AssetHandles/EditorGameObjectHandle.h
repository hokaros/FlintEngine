#pragma once
#include <Core/GameObject.h>
#include <AssetHandles/IHierarchyEditable.h>
#include <AssetHandles/ISaveable.h>

class EditorGameObjectHandle
	: public ISaveable
	, public IHierarchyEditable
{
public:
	virtual GameObject& GetGameObject() const = 0;

	// Modifying the underlying object
	// TODO: rename to SetWorldPosition
	void SetPosition(const Vector& pos);
	void SetLocalPosition(const Vector& pos);

	void SetSize(const Vector& size);
	void SetLocalScale(const Vector& scale);

	void SetName(const std::string& name);

	void AddComponent(std::unique_ptr<ObjectComponent> component);
	void ModifyComponentField(std::unique_ptr<ComponentFieldChange> modification);
	void RemoveComponent(size_t component_index);

	// IHierarchyEditable
	virtual const std::vector<std::unique_ptr<GameObject>>& GetSubRootObjects() const override;
	virtual void AddChild(std::unique_ptr<GameObject> child) override;
	virtual void DeleteChild(GameObject& child) override;
	virtual const char* GetName() const override;

	bool operator==(const EditorGameObjectHandle& other) const;
};

class EditorPrefabHandle
	: public EditorGameObjectHandle
{
public:
	EditorPrefabHandle(std::unique_ptr<GameObject> prefab, const std::string& prefab_path);

	virtual GameObject& GetGameObject() const override;

	// ISaveable
	virtual void Save() override;
	virtual bool HasUnsavedChanges() const override;
	virtual void OnUnsavedChange() override;

private:
	std::unique_ptr<GameObject> m_Prefab;
	std::string m_PrefabPath;

	bool m_HasUnsavedChanges = false;
};

class EditorIEditableGameObjectHandle
	: public EditorGameObjectHandle
{
public:
	EditorIEditableGameObjectHandle(GameObject& game_object, ISaveable& root_asset);

	virtual GameObject& GetGameObject() const override;

	// ISaveable
	virtual void Save() override;
	virtual bool HasUnsavedChanges() const override;
	virtual void OnUnsavedChange() override;

private:
	GameObject& m_EditableObject;
	ISaveable& m_RootAsset;
};