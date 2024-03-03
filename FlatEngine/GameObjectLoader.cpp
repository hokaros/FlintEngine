#include "GameObjectLoader.h"

#include <fstream>
#include <string>
#include "../FlatEngine/utility.h"
#include "../FlatEngine/GameObjectSerializer.h"

std::unique_ptr<GameObject> GameObjectLoader::LoadPrefab(const char* file_path)
{
    std::fstream prefab_file;
    prefab_file.open(file_path, std::ios::in);
    if (!prefab_file.is_open())
    {
        FE_LOG("Couldn't open prefab file: %s", file_path);
        return nullptr;
    }

    GameObjectLoader prefab_loader(prefab_file, 0);
    std::unique_ptr<GameObject> prefab = prefab_loader.LoadPrefab();

    prefab_file.close();
    return prefab;
}

GameObjectLoader::GameObjectLoader(std::fstream& file, size_t start_indent)
    : m_File(file)
    , m_StartIndent(start_indent)
    , m_PrevIndent(start_indent)
    , m_CurrIndent(start_indent)
    , m_ReturnedLine("")
{

}

std::unique_ptr<GameObject> GameObjectLoader::LoadPrefab()
{
    std::string first_unconsumed_line;

    std::unique_ptr<GameObjectStringDesc> prefab_serialized = LoadGameObject(first_unconsumed_line);
    FE_ASSERT(first_unconsumed_line == "", "All lines should be consumed");

    return GameObjectSerializer::DeserializeGameObject(*prefab_serialized);
}

std::unique_ptr<GameObjectStringDesc> GameObjectLoader::LoadGameObject(std::string& first_unconsumed_line)
{
    first_unconsumed_line = "";

    FE_ASSERT(m_GameObjectDesc == nullptr, "Previous GameObject hasn't been moved");

    m_GameObjectDesc = std::make_unique<GameObjectStringDesc>();

    char line[256];
    while (!m_File.eof())
    {
        std::string line_str;
        if (m_ReturnedLine.size() > 0)
        {
            line_str = m_ReturnedLine;
            m_ReturnedLine = "";
        }
        else
        {
            m_File.getline(line, 256);
            line_str = line;
        }

        bool line_consumed = DispatchLine(line_str);
        if (!line_consumed)
        {
            first_unconsumed_line = line_str;
            break;
        }
    }

    // Go to the most outer state
    while (m_ParsingState != ParsingState::GameObjectParams)
    {
        GoToOuterParsingState(m_PrevIndent);
    }

    return std::move(m_GameObjectDesc);
}

bool GameObjectLoader::DispatchLine(const std::string& line)
{
    m_CurrIndent = line.find('-');
    if (m_CurrIndent >= line.size() - 1)
    {
        if (line.size() > 0)
        {
            FE_DATA_ERROR("No '-' at the start of line");
        }
        return true;
    }

    if (m_CurrIndent < m_StartIndent)
    {
        // Scope of the outer GameObject
        return false;
    }

    std::string line_unindented = line.substr(m_CurrIndent + 1);
    
    if (m_CurrIndent < m_PrevIndent)
    {
        GoToOuterParsingState(m_PrevIndent - m_CurrIndent);
    }
    else if (m_CurrIndent > m_PrevIndent)
    {
        SetParsingState(m_ParsingStateAfterIndent);
    }
    else
    {
        SetParsingState(m_ParsingState);
    }

    ParseLineForCurrentState(line_unindented);

    m_PrevIndent = m_CurrIndent;

    return true;
}

void GameObjectLoader::ParseGameObjectParamLine(const std::string& line)
{
    std::string key, value;
    SplitLineToKeyAndValue(line, key, value);
    
    if (key == "components")
    {
        FE_DATA_CHECK(value.size() == 0, "Inline value for components detected");

        SetParsingStateAfterIndent(ParsingState::ComponentDefinitions);
    }
    else if (key == "children")
    {
        SetParsingStateAfterIndent(ParsingState::ChildDefinitions);
    }
    else
    {
        m_GameObjectDesc->params.insert({ key, value });
    }
}

void GameObjectLoader::ParseComponentNameLine(const std::string& line)
{
    std::string key, value;
    SplitLineToKeyAndValue(line, key, value);
    FE_DATA_CHECK(value.size() == 0, "Value after component name detected");

    m_CurrComponentDesc.type = key;

    SetParsingStateAfterIndent(ParsingState::SpecificComponentDefinition);
}

void GameObjectLoader::ParseComponentFieldLine(const std::string& line)
{
    std::string key, value;
    SplitLineToKeyAndValue(line, key, value);

    m_CurrComponentDesc.fields.insert({ key, value });
}

void GameObjectLoader::FinalizeComponentLoading()
{
    m_GameObjectDesc->components.push_back(std::make_unique<ComponentStringDesc>(m_CurrComponentDesc));

    m_CurrComponentDesc.type = "";
    m_CurrComponentDesc.fields.clear();
}

void GameObjectLoader::ParseChildTypeLine(const std::string& line)
{
    std::string line_trimmed = line;
    TrimWhitespaces(line_trimmed);
    if (line_trimmed == "GameObject")
    {
        GameObjectLoader child_loader(m_File, m_CurrIndent + 1);
        m_GameObjectDesc->children.push_back(child_loader.LoadGameObject(m_ReturnedLine));
    }
    else // TODO: prefab reference
    {
        FE_DATA_ERROR("Unknown GameObject type");
    }
}

void GameObjectLoader::SetParsingState(ParsingState state)
{
    bool is_loading_component = m_ParsingState == ParsingState::SpecificComponentDefinition || m_ParsingStateAfterIndent == ParsingState::SpecificComponentDefinition;
    if (state != ParsingState::SpecificComponentDefinition && is_loading_component)
    {
        FinalizeComponentLoading();
    }

    m_ParsingState = state;
    m_ParsingStateAfterIndent = state;
}

void GameObjectLoader::SetParsingStateAfterIndent(ParsingState state)
{
    m_ParsingStateAfterIndent = state;
}

void GameObjectLoader::GoToOuterParsingState(size_t levels)
{
    for (size_t i = 0; i < levels; i++)
    {
        switch (m_ParsingState)
        {
        case ParsingState::ComponentDefinitions:
        case ParsingState::ChildDefinitions:
            SetParsingState(ParsingState::GameObjectParams);
            break;
        case ParsingState::SpecificComponentDefinition:
            SetParsingState(ParsingState::ComponentDefinitions);
            break;
        }
    }
}

void GameObjectLoader::ParseLineForCurrentState(const std::string& line)
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
    case ParsingState::ChildDefinitions:
        ParseChildTypeLine(line);
        break;
    default:
        FE_ASSERT(false, "Missing parsing state");
    }
}

void GameObjectLoader::SplitLineToKeyAndValue(const std::string& line, std::string& key, std::string& value)
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

void GameObjectLoader::TrimWhitespaces(std::string& symbol)
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
