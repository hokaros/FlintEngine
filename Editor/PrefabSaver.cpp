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

    PrefabSaver saver(prefab_file, 0);
    saver.SavePrefab(prefab);

    prefab_file.close();
}

PrefabSaver::PrefabSaver(std::fstream& prefab_file, size_t start_indent)
    : m_PrefabFile(prefab_file)
    , m_StartIndent(start_indent)
    , m_IndentPrinter(prefab_file, start_indent)
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
    m_IndentPrinter.PrintLine("components:");
    m_IndentPrinter.IncreaseIndent();

    for (const std::unique_ptr<ComponentStringDesc>& component : components)
    {
        m_IndentPrinter.StartLine();
        m_IndentPrinter.AddToLine(component->type);

        if (component->fields.size() > 0)
        {
            m_IndentPrinter.AddToLine(":");
        }
        m_IndentPrinter.EndLine();

        m_IndentPrinter.IncreaseIndent();
        SaveKeyValuePairs(component->fields);
        m_IndentPrinter.DecreaseIndent();
    }

    m_IndentPrinter.DecreaseIndent();
}

void PrefabSaver::SaveKeyValuePairs(const std::map<std::string, std::string>& dict)
{
    for (auto& pair : dict)
    {
        const std::string& key = pair.first;
        const std::string& value = pair.second;

        m_IndentPrinter.StartLine();
        m_IndentPrinter.AddToLine(key);
        m_IndentPrinter.AddToLine(": ");
        m_IndentPrinter.AddToLine(value);
        m_IndentPrinter.EndLine();
    }
}