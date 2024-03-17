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



EditorUniversalHandle::EditorUniversalHandle(std::shared_ptr<EditorGameObjectHandle> game_object)
    : m_EditableGameObject(std::move(game_object))
    , m_PrefabHandle(nullptr)
{
    m_HierarchyEditable = m_EditableGameObject->GetGameObject();
}

EditorUniversalHandle::EditorUniversalHandle(std::shared_ptr<EditorPrefabHandle> prefab)
    : m_PrefabHandle(std::move(prefab))
    , m_EditableGameObject(prefab)
{
    m_HierarchyEditable = m_EditableGameObject->GetGameObject();
}

IHierarchyEditable* EditorUniversalHandle::GetHierarchyEditable() const
{
    return m_HierarchyEditable;
}

std::shared_ptr<EditorGameObjectHandle> EditorUniversalHandle::GetGameObjectHandle() const
{
    return m_EditableGameObject;
}
