#include "PrefabSaver.h"
#include <fstream>

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



void PrefabSaver::SavePrefab(const GameObject& prefab, const char* file_path)
{
    std::fstream prefab_file;
    prefab_file.open(file_path, std::ios::out);
    if (!prefab_file.is_open())
    {
        FE_LOG("Couldn't save prefab file: %s", file_path);
        return;
    }

    PrefabSaver saver(prefab_file);
    saver.SavePrefab(prefab);

    prefab_file.close();
}

PrefabSaver::PrefabSaver(std::fstream& prefab_file)
    : m_PrefabFile(prefab_file)
{

}

void PrefabSaver::SavePrefab(const GameObject& prefab)
{
    std::unique_ptr<GameObjectStringDesc> prefab_serialized = GameObjectSerializer::SerializeGameObject(prefab);
    SaveKeyValuePairs(prefab_serialized->params);
}

void PrefabSaver::SaveKeyValuePairs(const std::map<std::string, std::string>& dict)
{
    for (auto& pair : dict)
    {
        const std::string& key = pair.first;
        const std::string& value = pair.second;

        m_PrefabFile << " - " << key << ": " << value << std::endl;
    }
}
