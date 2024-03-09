#include "AssetHandles.h"

#include "PrefabSaver.h"

EditorPrefabHandle::EditorPrefabHandle(std::unique_ptr<GameObject> prefab, const std::string& prefab_path)
    : m_Prefab(std::move(prefab))
    , m_PrefabPath(prefab_path)
{
    m_EditablePrefab = std::make_unique<PlainGameObject>(*m_Prefab);
}

IEditableGameObject* EditorPrefabHandle::GetGameObject() const
{
    return m_EditablePrefab.get();
}

void EditorPrefabHandle::SaveGameObject()
{
    if (m_Prefab != nullptr)
    {
        PrefabSaver::SavePrefab(*m_Prefab, m_PrefabPath.c_str());
    }
}

EditorPlainGameObjectHandle::EditorPlainGameObjectHandle(GameObject* game_object)
{
    if (game_object == nullptr)
    {
        m_EditableObject = nullptr;
    }
    else
    {
        m_EditableObject = std::make_unique<PlainGameObject>(*game_object);
    }
}

IEditableGameObject* EditorPlainGameObjectHandle::GetGameObject() const
{
    return m_EditableObject.get();
}

void EditorPlainGameObjectHandle::SaveGameObject()
{
    // Pass
}
