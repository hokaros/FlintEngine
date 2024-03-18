#include "AssetHandles.h"

#include "PrefabSaver.h"

EditorPrefabHandle::EditorPrefabHandle(std::unique_ptr<InlineGameObject> prefab, const std::string& prefab_path)
    : m_Prefab(std::move(prefab))
    , m_PrefabPath(prefab_path)
{

}

IEditableGameObject* EditorPrefabHandle::GetGameObject() const
{
    return m_Prefab.get();
}

void EditorPrefabHandle::SaveInlineGameObject()
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

void EditorIEditableGameObjectHandle::SaveInlineGameObject()
{

}


EditorSceneHandle::EditorSceneHandle(std::unique_ptr<EditableScene> scene, const std::string& scene_path)
    : m_Scene(std::move(scene))
    , m_ScenePath(scene_path)
{
}

EditableScene* EditorSceneHandle::GetScene() const
{
    return m_Scene.get();
}

void EditorSceneHandle::SaveScene()
{
    FE_ASSERT(false, "Unimplemented");
}




EditorUniversalHandle::EditorUniversalHandle(std::shared_ptr<EditorGameObjectHandle> game_object)
    : m_EditableGameObject(std::move(game_object))
{
    m_HierarchyEditable = m_EditableGameObject->GetGameObject();
}

EditorUniversalHandle::EditorUniversalHandle(std::shared_ptr<EditorPrefabHandle> prefab)
    : m_EditableGameObject(std::dynamic_pointer_cast<EditorGameObjectHandle>(prefab))
{
    m_HierarchyEditable = m_EditableGameObject->GetGameObject();
}

EditorUniversalHandle::EditorUniversalHandle(std::shared_ptr<EditorSceneHandle> scene)
    : m_SceneHandle(std::move(scene))
{
    m_HierarchyEditable = m_SceneHandle->GetScene();
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
