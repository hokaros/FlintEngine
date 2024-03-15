#include "PrefabSaver.h"
#include <fstream>



void PrefabSaver::SavePrefab(const InlineGameObject& prefab, const char* file_path)
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
    : m_IndentPrinter(prefab_file, start_indent)
{

}

void PrefabSaver::SavePrefab(const InlineGameObject& prefab)
{
    std::unique_ptr<GameObjectStringDesc> prefab_serialized = GameObjectSerializer::SerializeGameObject(prefab);
    SaveInlineGameObject(*prefab_serialized);
}

void PrefabSaver::SaveGameObject(const GameObjectStringDescProxy& game_object)
{
    switch (game_object.GetType())
    {
    case EditableGameObjectType::InlineGameObject:
        m_IndentPrinter.PrintLine("GameObject");
        m_IndentPrinter.IncreaseIndent();

        SaveInlineGameObject(static_cast<const InlineGameObjectStringDescEndpoint&>(game_object).GetDesc());

        m_IndentPrinter.DecreaseIndent();
        break;

    case EditableGameObjectType::PrefabInstance:
        m_IndentPrinter.PrintLine("PrefabInstance");
        m_IndentPrinter.IncreaseIndent();

        SavePrefabInstance(static_cast<const PrefabInstanceStringDescEndpoint&>(game_object).GetDesc());

        m_IndentPrinter.DecreaseIndent();
        break;

    default:
        FE_ASSERT(false, "Incomplete proxying");
    }
}

void PrefabSaver::SaveInlineGameObject(const GameObjectStringDesc& game_object)
{
    SaveKeyValuePairs(game_object.params);

    SaveComponents(game_object.components);

    SaveChildren(game_object.children);
}

void PrefabSaver::SavePrefabInstance(const PrefabInstanceStringDesc& prefab_instance)
{
    SaveKeyValuePair(PrefabInstanceSerializer::s_PrefabKey, prefab_instance.m_PrefabPath);

    SaveKeyValuePairs(prefab_instance.m_OverridenParams);

    // TODO: additional children

    //SaveComponents(prefab_instance.m_AdditionalComponents);
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

void PrefabSaver::SaveChildren(const std::vector<std::unique_ptr<GameObjectStringDescProxy>>& children)
{
    m_IndentPrinter.PrintLine("children:");
    m_IndentPrinter.IncreaseIndent();

    for (const std::unique_ptr<GameObjectStringDescProxy>& child : children)
    {
        SaveGameObject(*child);
    }

    m_IndentPrinter.DecreaseIndent();
}

void PrefabSaver::SaveKeyValuePair(const std::string& key, const std::string& value)
{
    m_IndentPrinter.StartLine();
    m_IndentPrinter.AddToLine(key);
    m_IndentPrinter.AddToLine(": ");
    m_IndentPrinter.AddToLine(value);
    m_IndentPrinter.EndLine();
}

void PrefabSaver::SaveKeyValuePairs(const std::map<std::string, std::string>& dict)
{
    for (auto& pair : dict)
    {
        const std::string& key = pair.first;
        const std::string& value = pair.second;

        SaveKeyValuePair(key, value);
    }
}