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
    : m_IndentPrinter(prefab_file, start_indent)
{

}

PrefabSaver::PrefabSaver(const IndentPrinter& indent_printer)
    : m_IndentPrinter(indent_printer)
{
}

void PrefabSaver::SavePrefab(const GameObject& prefab)
{
    std::unique_ptr<GameObjectStringDesc> prefab_serialized = GameObjectSerializer::SerializeGameObject(prefab);
    SaveInlineGameObject(*prefab_serialized);
}

void PrefabSaver::SaveGameObject(const GameObjectStringDescProxy& game_object)
{
    switch (game_object.GetType())
    {
    case GameObjectType::GameObject:
        m_IndentPrinter.PrintLine("GameObject");
        m_IndentPrinter.IncreaseIndent();

        SaveInlineGameObject(static_cast<const InlineGameObjectStringDescEndpoint&>(game_object).GetDesc());

        m_IndentPrinter.DecreaseIndent();
        break;

    case GameObjectType::PrefabInstance:
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
    m_IndentPrinter.SaveKeyValuePairs(game_object.params);

    SaveComponents(game_object.components);

    SaveChildren(game_object.children);
}

void PrefabSaver::SavePrefabInstance(const PrefabInstanceStringDesc& prefab_instance)
{
    m_IndentPrinter.SaveKeyValuePair(PrefabInstanceSerializer::s_PrefabKey, prefab_instance.m_PrefabPath);

    m_IndentPrinter.SaveKeyValuePairs(prefab_instance.m_OverridenParams);

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
        m_IndentPrinter.SaveKeyValuePairs(component->fields);
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