#include "AssetHandles.h"

#include "PrefabSaver.h"

EditorGameObjectHandle::EditorGameObjectHandle(GameObject* game_object)
    : m_GameObject(game_object)
{
}

GameObject* EditorGameObjectHandle::GetGameObject() const
{
    return m_GameObject;
}

void EditorGameObjectHandle::SaveGameObject()
{
}

EditorPrefabHandle::EditorPrefabHandle(std::unique_ptr<GameObject> prefab, const std::string& prefab_path)
    : EditorGameObjectHandle(prefab.get())
    , m_Prefab(std::move(prefab))
    , m_PrefabPath(prefab_path)
{
}

void EditorPrefabHandle::SaveGameObject()
{
    __super::SaveGameObject();

    if (m_GameObject != nullptr)
    {
        PrefabSaver::SavePrefab(*m_GameObject, m_PrefabPath.c_str());
    }
}
