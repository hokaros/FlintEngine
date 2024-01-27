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

    SaveComponents(prefab);
}

void PrefabSaver::SaveComponents(const GameObject& prefab)
{
    m_PrefabFile << "- components:" << std::endl;

    for (const ObjectComponent* component : prefab.GetAllComponents())
    {
        ComponentDefinition* comp_def = ComponentDefinitionManager::GetInstance().GetDefinition(*component);

        m_PrefabFile << "\t- " << comp_def->GetName();
        if (comp_def->GetFields().size() > 0)
        {
            m_PrefabFile << ':';
        }
        m_PrefabFile << std::endl;

        SaveFields(*component);
    }
}

void PrefabSaver::SaveFields(const ObjectComponent& component)
{
    ComponentDefinition* comp_def = ComponentDefinitionManager::GetInstance().GetDefinition(component);

    for (const ComponentFieldDefinition* field : comp_def->GetFields())
    {
        m_PrefabFile << "\t\t- ";
        m_PrefabFile << field->GetFieldName() << ": " << field->GetFieldValue(&component) << std::endl;
    }
}

void PrefabSaver::SaveKeyValuePairs(const std::map<std::string, std::string>& dict)
{
    for (auto& pair : dict)
    {
        const std::string& key = pair.first;
        const std::string& value = pair.second;

        m_PrefabFile << "- " << key << ": " << value << std::endl;
    }
}
