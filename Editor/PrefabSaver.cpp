#include "PrefabSaver.h"
#include <fstream>



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

    SaveComponents(prefab_serialized->components);
}

void PrefabSaver::SaveComponents(const std::vector<std::unique_ptr<ComponentStringDesc>>& components)
{
    m_PrefabFile << "- components:" << std::endl;

    for (const std::unique_ptr<ComponentStringDesc>& component : components)
    {
        m_PrefabFile << "\t- " << component->type;
        if (component->fields.size() > 0)
        {
            m_PrefabFile << ':';
        }
        m_PrefabFile << std::endl;

        SaveFields(component->fields);
    }
}

void PrefabSaver::SaveFields(const std::map<std::string, std::string>& fields)
{
    SaveKeyValuePairs(fields, /*indent*/2);
}

void PrefabSaver::SaveKeyValuePairs(const std::map<std::string, std::string>& dict, size_t indent)
{
    std::string indent_str = "";
    for (size_t i = 0; i < indent; i++)
    {
        indent_str += "\t";
    }

    for (auto& pair : dict)
    {
        const std::string& key = pair.first;
        const std::string& value = pair.second;

        m_PrefabFile << indent_str << "- " << key << ": " << value << std::endl;
    }
}
