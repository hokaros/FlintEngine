#include "AssetHandles.h"

#include "PrefabSaver.h"
#include "SceneSaver.h"

bool EditorGameObjectHandle::operator==(const EditorGameObjectHandle& other) const
{
    return GetGameObject() == other.GetGameObject();
}


EditorPrefabHandle::EditorPrefabHandle(std::unique_ptr<InlineGameObject> prefab, const std::string& prefab_path)
    : m_Prefab(std::move(prefab))
    , m_PrefabPath(prefab_path)
{

}

IEditableGameObject* EditorPrefabHandle::GetGameObject() const
{
    return m_Prefab.get();
}

void EditorPrefabHandle::Save()
{
    if (m_Prefab != nullptr)
    {
        PrefabSaver::SavePrefab(*m_Prefab, m_PrefabPath.c_str());
    }
}


EditorIEditableGameObjectHandle::EditorIEditableGameObjectHandle(IEditableGameObject* game_object)
    : m_EditableObject(game_object)
{
}

IEditableGameObject* EditorIEditableGameObjectHandle::GetGameObject() const
{
    return m_EditableObject;
}

void EditorIEditableGameObjectHandle::Save()
{

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
}

bool EditorSceneHandle::operator==(const EditorSceneHandle& other) const
{
    return m_Scene.get() == other.m_Scene.get();
}




EditorUniversalHandle::EditorUniversalHandle(std::shared_ptr<EditorGameObjectHandle> game_object)
    : m_EditableGameObject(std::move(game_object))
{
    m_HierarchyEditable = m_EditableGameObject->GetGameObject();
    m_Saveable = m_EditableGameObject.get();
}

EditorUniversalHandle::EditorUniversalHandle(std::shared_ptr<EditorIEditableGameObjectHandle> game_object)
    : m_EditableGameObject(std::static_pointer_cast<EditorGameObjectHandle>(game_object))
{
    m_HierarchyEditable = m_EditableGameObject->GetGameObject();
    m_Saveable = m_EditableGameObject.get();
}

EditorUniversalHandle::EditorUniversalHandle(std::shared_ptr<EditorSceneHandle> scene)
    : m_SceneHandle(std::move(scene))
{
    m_HierarchyEditable = &m_SceneHandle->GetScene();
    m_Saveable = m_SceneHandle.get();
}

IHierarchyEditable* EditorUniversalHandle::GetHierarchyEditable() const
{
    return m_HierarchyEditable;
}

std::shared_ptr<EditorGameObjectHandle> EditorUniversalHandle::GetGameObjectHandle() const
{
    return m_EditableGameObject;
}

std::shared_ptr<EditorSceneHandle> EditorUniversalHandle::GetSceneHandle() const
{
    return m_SceneHandle;
}

void EditorUniversalHandle::Save()
{
    m_Saveable->Save();
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
