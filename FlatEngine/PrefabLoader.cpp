#include "PrefabLoader.h"
#include <fstream>
#include <string>
#include "../FlatEngine/utility.h"
#include "../FlatEngine/GameObjectSerializer.h"

std::unique_ptr<GameObject> PrefabLoader::LoadPrefab(const char* file_path)
{
    std::fstream prefab_file;
    prefab_file.open(file_path, std::ios::in);
    if (!prefab_file.is_open())
    {
        FE_LOG("Couldn't open prefab file: %s", file_path);
        return nullptr;
    }

    PrefabLoader prefab_loader;
    std::unique_ptr<GameObject> prefab = prefab_loader.LoadPrefab(prefab_file);

    prefab_file.close();
    return prefab;
}

std::unique_ptr<GameObject> PrefabLoader::LoadPrefab(std::fstream& file)
{
    char line[256];
    while (!file.eof())
    {
        file.getline(line, 256);
        DispatchLine(line);
    }

    // Go to the most outer state
    while (m_ParsingState != ParsingState::GameObjectParams)
    {
        GoToOuterParsingState();
    }

    return GameObjectSerializer::DeserializeGameObject(m_GameObjectDesc);
}

void PrefabLoader::DispatchLine(const std::string& line)
{
    size_t curr_indent = line.find('-');
    if (curr_indent >= line.size() - 1)
        return;

    std::string line_unindented = line.substr(curr_indent + 1);
    
    if (curr_indent == 0 && m_ParsingState != ParsingState::GameObjectParams)
    {
        SetParsingState(ParsingState::GameObjectParams);
    }
    else if (curr_indent < m_PrevIndent)
    {
        GoToOuterParsingState();
    }
    else if (curr_indent > m_PrevIndent)
    {
        SetParsingState(m_ParsingStateAfterIndent);
    }
    else
    {
        SetParsingState(m_ParsingState);
    }

    ParseLineForCurrentState(line_unindented);

    m_PrevIndent = curr_indent;
}

void PrefabLoader::ParseGameObjectParamLine(const std::string& line)
{
    std::string key, value;
    SplitLineToKeyAndValue(line, key, value);
    
    if (key == "components")
    {
        FE_DATA_CHECK(value.size() == 0, "Inline value for components detected");

        SetParsingStateAfterIndent(ParsingState::ComponentDefinitions);
    }
    else
    {
        m_GameObjectDesc.params.insert({ key, value });
    }
}

void PrefabLoader::ParseComponentNameLine(const std::string& line)
{
    std::string key, value;
    SplitLineToKeyAndValue(line, key, value);
    FE_DATA_CHECK(value.size() == 0, "Value after component name detected");

    m_CurrComponentDesc.type = key;

    SetParsingStateAfterIndent(ParsingState::SpecificComponentDefinition);
}

void PrefabLoader::ParseComponentFieldLine(const std::string& line)
{
    std::string key, value;
    SplitLineToKeyAndValue(line, key, value);

    m_CurrComponentDesc.fields.insert({ key, value });
}

void PrefabLoader::FinalizeComponentLoading()
{
    m_GameObjectDesc.components.push_back(m_CurrComponentDesc);

    m_CurrComponentDesc.type = "";
    m_CurrComponentDesc.fields.clear();
}

void PrefabLoader::SetParsingState(ParsingState state)
{
    bool is_loading_component = m_ParsingState == ParsingState::SpecificComponentDefinition || m_ParsingStateAfterIndent == ParsingState::SpecificComponentDefinition;
    if (state != ParsingState::SpecificComponentDefinition && is_loading_component)
    {
        FinalizeComponentLoading();
    }

    m_ParsingState = state;
    m_ParsingStateAfterIndent = state;
}

void PrefabLoader::SetParsingStateAfterIndent(ParsingState state)
{
    m_ParsingStateAfterIndent = state;
}

void PrefabLoader::GoToOuterParsingState()
{
    switch (m_ParsingState)
    {
    case ParsingState::ComponentDefinitions:
        SetParsingState(ParsingState::GameObjectParams);
        break;
    case ParsingState::SpecificComponentDefinition:
        SetParsingState(ParsingState::ComponentDefinitions);
        break;
    }
}

void PrefabLoader::ParseLineForCurrentState(const std::string& line)
{
    switch (m_ParsingState)
    {
    case ParsingState::GameObjectParams:
        ParseGameObjectParamLine(line);
        break;
    case ParsingState::ComponentDefinitions:
        ParseComponentNameLine(line);
        break;
    case ParsingState::SpecificComponentDefinition:
        ParseComponentFieldLine(line);
        break;
    default:
        FE_ASSERT(false, "Missing parsing state");
    }
}

void PrefabLoader::SplitLineToKeyAndValue(const std::string& line, std::string& key, std::string& value)
{
    size_t colon_pos = line.find(':');

    key = line.substr(0, colon_pos);
    TrimWhitespaces(key);
    if (colon_pos == line.size() - 1 ||
        colon_pos == (size_t)-1)
    {
        value = "";
        return;
    }

    value = line.substr(colon_pos + 1);
    TrimWhitespaces(value);
}

void PrefabLoader::TrimWhitespaces(std::string& symbol)
{
    // Trim from left
    symbol.erase(
        symbol.begin(), 
        std::find_if(symbol.begin(), symbol.end(), [](unsigned char c) {
            return !std::isspace(c);
        })
    );

    // Trim from right
    symbol.erase(
        std::find_if(symbol.rbegin(), symbol.rend(), [](unsigned char c) {
            return !std::isspace(c);
        }).base(),
        symbol.end()
    );
}
