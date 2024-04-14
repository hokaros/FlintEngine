#include "AssetHandles.h"

#include <Serialization/PrefabSaver.h>
#include <Serialization/SceneSaver.h>
#include <AssetManager.h>

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


EditorSceneHandle::EditorSceneHandle(std::unique_ptr<EditableScene> scene, const std::string& scene_path)
    : m_Scene(std::move(scene))
    , m_ScenePath(scene_path)
{
    FE_ASSERT(m_Scene != nullptr, "No scene passed to SceneHandle");
}

EditableScene& EditorSceneHandle::GetScene() const
{
    return *m_Scene;
}

void EditorSceneHandle::Save()
{
    if (m_Scene != nullptr)
    {
        SceneSaver::SaveScene(*m_Scene, m_ScenePath.c_str());
    }

    m_HasUnsavedChanges = false;
}

bool EditorSceneHandle::HasUnsavedChanges() const
{
    return m_HasUnsavedChanges;
}

void EditorSceneHandle::OnUnsavedChange()
{
    m_HasUnsavedChanges = true;
}

bool EditorSceneHandle::operator==(const EditorSceneHandle& other) const
{
    return m_Scene.get() == other.m_Scene.get();
}




EditorUniversalHandle::EditorUniversalHandle(std::shared_ptr<EditorGameObjectHandle> game_object)
    : m_EditableGameObject(std::move(game_object))
{
    FE_ASSERT(m_EditableGameObject != nullptr, "Creating handle without GameObject");

    m_HierarchyEditable = &m_EditableGameObject->GetGameObject();
    m_Saveable = m_EditableGameObject.get();
}

EditorUniversalHandle::EditorUniversalHandle(std::shared_ptr<EditorIEditableGameObjectHandle> game_object)
    : m_EditableGameObject(std::static_pointer_cast<EditorGameObjectHandle>(game_object))
{
    FE_ASSERT(m_EditableGameObject != nullptr, "Creating handle without GameObject");

    m_HierarchyEditable = &m_EditableGameObject->GetGameObject();
    m_Saveable = m_EditableGameObject.get();
}

EditorUniversalHandle::EditorUniversalHandle(std::shared_ptr<EditorSceneHandle> scene)
    : m_SceneHandle(std::move(scene))
{
    FE_ASSERT(m_SceneHandle != nullptr, "Creating handle without SceneHandle");

    m_HierarchyEditable = &m_SceneHandle->GetScene();
    m_Saveable = m_SceneHandle.get();
}

IHierarchyEditable* EditorUniversalHandle::GetHierarchyEditable() const
{
    return m_HierarchyEditable;
}

EditorGameObjectHandle* EditorUniversalHandle::GetGameObjectHandle() const
{
    return m_EditableGameObject.get();
}

EditorSceneHandle* EditorUniversalHandle::GetSceneHandle() const
{
    return m_SceneHandle.get();
}

void EditorUniversalHandle::Save()
{
    m_Saveable->Save();
}

bool EditorUniversalHandle::HasUnsavedChanges() const
{
    return m_Saveable->HasUnsavedChanges();
}

void EditorUniversalHandle::OnUnsavedChange()
{
    return m_Saveable->OnUnsavedChange();
}

bool EditorUniversalHandle::operator==(const EditorUniversalHandle& other) const
{
    return AreGameObjectHandlesSame(other)
        && AreSceneHandlesSame(other);
}

bool EditorUniversalHandle::AreGameObjectHandlesSame(const EditorUniversalHandle& other) const
{
    return ArePointedObjectsEqual(m_EditableGameObject.get(), other.m_EditableGameObject.get());
}

bool EditorUniversalHandle::AreSceneHandlesSame(const EditorUniversalHandle& other) const
{
    return ArePointedObjectsEqual(m_SceneHandle.get(), other.m_SceneHandle.get());
}
