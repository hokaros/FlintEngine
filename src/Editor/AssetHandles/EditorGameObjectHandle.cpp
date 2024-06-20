#include "EditorGameObjectHandle.h"

#include <Serialization/PrefabSaver.h>
#include <AssetManager.h>


const GameObject& EditorGameObjectHandle::GetResult_Depr() const
{
    return GetGameObject().GetResult_Depr();
}

void EditorGameObjectHandle::SetPosition(const Vector& pos)
{
    GetGameObject().GetTransformable().SetWorldPosition(pos);
    OnUnsavedChange();
}

void EditorGameObjectHandle::SetSize(const Vector& size)
{
    GetGameObject().GetTransformable().SetWorldScale(size);
    OnUnsavedChange();
}

void EditorGameObjectHandle::SetName(const std::string& name)
{
    GetGameObject().SetName(name);
    OnUnsavedChange();
}

void EditorGameObjectHandle::AddComponent(std::unique_ptr<ObjectComponent> component)
{
    GetGameObject().AddComponent(std::move(component));
    OnUnsavedChange();
}

void EditorGameObjectHandle::ModifyComponentField(std::unique_ptr<ComponentFieldChange> modification)
{
    GetGameObject().Serializable_ModifyComponentField(std::move(modification));
    OnUnsavedChange();
}

void EditorGameObjectHandle::RemoveComponent(size_t component_index)
{
    GetGameObject().RemoveComponent(component_index);
    OnUnsavedChange();
}

const std::vector<std::unique_ptr<IGameObject>>& EditorGameObjectHandle::GetSubRootObjects() const
{
    return GetGameObject().GetChildren();
}

void EditorGameObjectHandle::AddChild(std::unique_ptr<IEditableGameObject> child)
{
    GetGameObject().AddChild(std::move(child));
    OnUnsavedChange();
}

void EditorGameObjectHandle::DeleteChild(IEditableGameObject& child)
{
    GetGameObject().RemoveChild(child);
    OnUnsavedChange();
}

const char* EditorGameObjectHandle::GetName() const
{
    return GetGameObject().GetName().c_str();
}

bool EditorGameObjectHandle::operator==(const EditorGameObjectHandle& other) const
{
    return &GetGameObject() == &other.GetGameObject();
}


EditorPrefabHandle::EditorPrefabHandle(std::unique_ptr<InlineGameObject> prefab, const std::string& prefab_path)
    : m_Prefab(std::move(prefab))
    , m_PrefabPath(prefab_path)
{
    FE_ASSERT(m_Prefab != nullptr, "No prefab passed to PrefabHandle");
}

IEditableGameObject& EditorPrefabHandle::GetGameObject() const
{
    return *m_Prefab;
}

void EditorPrefabHandle::Save()
{
    if (m_Prefab != nullptr)
    {
        PrefabSaver::SavePrefab(*m_Prefab, m_PrefabPath.c_str());

        // TODO: EditorPrefabHandle should have only the prefab path relative to root
        constexpr const char* root_dir = "Assets\\";
        std::string prefab_id = m_PrefabPath.substr(strlen(root_dir));
        AssetManager::GetInstance()->InvalidatePrefab(prefab_id);
    }

    m_HasUnsavedChanges = false;
}

bool EditorPrefabHandle::HasUnsavedChanges() const
{
    return m_HasUnsavedChanges;
}

void EditorPrefabHandle::OnUnsavedChange()
{
    m_HasUnsavedChanges = true;
}


EditorIEditableGameObjectHandle::EditorIEditableGameObjectHandle(IEditableGameObject& game_object, ISaveable& root_asset)
    : m_EditableObject(game_object)
    , m_RootAsset(root_asset)
{
}

IEditableGameObject& EditorIEditableGameObjectHandle::GetGameObject() const
{
    return m_EditableObject;
}

void EditorIEditableGameObjectHandle::Save()
{
    m_RootAsset.Save();
}

bool EditorIEditableGameObjectHandle::HasUnsavedChanges() const
{
    return m_RootAsset.HasUnsavedChanges();
}

void EditorIEditableGameObjectHandle::OnUnsavedChange()
{
    m_RootAsset.OnUnsavedChange();
}