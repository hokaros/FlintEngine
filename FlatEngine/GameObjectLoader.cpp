#include "GameObjectLoader.h"

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
    : IndentFileParser<GameObjectParsingState>(file, start_indent, GameObjectParsingState::GameObjectParams)
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
    FE_ASSERT(m_GameObjectDesc == nullptr, "Previous GameObject hasn't been moved");

    m_GameObjectDesc = std::make_unique<GameObjectStringDesc>();

    Load(first_unconsumed_line);

    FE_ASSERT(m_ParsingState == GameObjectParsingState::GameObjectParams, "Should have reached most outer parsing state");

    return std::move(m_GameObjectDesc);
}

void GameObjectLoader::ParseGameObjectParamLine(const std::string& line)
{
    std::string key, value;
    SplitLineToKeyAndValue(line, key, value);
    
    if (key == "components")
    {
        FE_DATA_CHECK(value.size() == 0, "Inline value for components detected");

        SetParsingStateAfterIndent(GameObjectParsingState::ComponentDefinitions);
    }
    else if (key == "children")
    {
        SetParsingStateAfterIndent(GameObjectParsingState::ChildDefinitions);
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

    SetParsingStateAfterIndent(GameObjectParsingState::SpecificComponentDefinition);
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

void GameObjectLoader::SetParsingState(GameObjectParsingState state)
{
    bool is_loading_component = m_ParsingState == GameObjectParsingState::SpecificComponentDefinition || m_ParsingStateAfterIndent == GameObjectParsingState::SpecificComponentDefinition;
    if (state != GameObjectParsingState::SpecificComponentDefinition && is_loading_component)
    {
        FinalizeComponentLoading();
    }

    IndentFileParser<GameObjectParsingState>::SetParsingState(state);
}

GameObjectParsingState GameObjectLoader::GetOuterParsingState(GameObjectParsingState current_state)
{
    switch (current_state)
    {
    case GameObjectParsingState::ComponentDefinitions:
    case GameObjectParsingState::ChildDefinitions:
        return GameObjectParsingState::GameObjectParams;

    case GameObjectParsingState::SpecificComponentDefinition:
        return GameObjectParsingState::ComponentDefinitions;

    default:
        return GameObjectParsingState::GameObjectParams;
    }
}

void GameObjectLoader::ParseLineForCurrentState(const std::string& line)
{
    switch (m_ParsingState)
    {
    case GameObjectParsingState::GameObjectParams:
        ParseGameObjectParamLine(line);
        break;
    case GameObjectParsingState::ComponentDefinitions:
        ParseComponentNameLine(line);
        break;
    case GameObjectParsingState::SpecificComponentDefinition:
        ParseComponentFieldLine(line);
        break;
    case GameObjectParsingState::ChildDefinitions:
        ParseChildTypeLine(line);
        break;
    default:
        FE_ASSERT(false, "Missing parsing state");
    }
}