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
