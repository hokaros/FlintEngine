#include "GameObjectLoader.h"

#include <string>
#include "utility.h"
#include "GameObjectSerializer.h"
#include "PrefabInstanceLoader.h"

std::unique_ptr<InlineGameObject> GameObjectLoader::LoadPrefab(const char* file_path)
{
    std::unique_ptr<GameObjectStringDesc> prefab_serialized = LoadPrefabDesc(file_path);
    if (prefab_serialized == nullptr)
        return nullptr;

    return GameObjectSerializer::DeserializeGameObject(*prefab_serialized);
}

std::unique_ptr<GameObjectStringDesc> GameObjectLoader::LoadPrefabDesc(const char* file_path)
{
    std::fstream prefab_file;
    prefab_file.open(file_path, std::ios::in);
    if (!prefab_file.is_open())
    {
        FE_LOG("Couldn't open prefab file: %s", file_path);
        return nullptr;
    }

    GameObjectLoader prefab_loader(prefab_file, 0);
    std::unique_ptr<GameObjectStringDesc> prefab = prefab_loader.LoadPrefabDesc();

    prefab_file.close();
    return prefab;
}

std::unique_ptr<GameObjectStringDescProxy> GameObjectLoader::ParseChildByKey(const char* key, std::fstream& file, size_t start_indent, std::string& first_unconsumed_line)
{
    if (key == "GameObject")
    {
        GameObjectLoader child_loader(file, start_indent + 1);
        std::unique_ptr<GameObjectStringDesc> child_desc = child_loader.LoadGameObject(first_unconsumed_line);
        return std::make_unique<InlineGameObjectStringDescEndpoint>(std::move(child_desc));
    }
    else if (key == "PrefabInstance")
    {
        PrefabInstanceLoader child_loader(file, start_indent + 1);
        std::unique_ptr<PrefabInstanceStringDesc> child_desc = child_loader.LoadPrefabInstanceDesc(first_unconsumed_line);
        return std::make_unique<PrefabInstanceStringDescEndpoint>(std::move(child_desc));
    }
    else
    {
        FE_DATA_ERROR("Unknown GameObject type");
        return nullptr;
    }
}

GameObjectLoader::GameObjectLoader(std::fstream& file, size_t start_indent)
    : IndentFileParser<GameObjectParsingState>(file, start_indent, GameObjectParsingState::GameObjectParams)
{
}

std::unique_ptr<GameObjectStringDesc> GameObjectLoader::LoadPrefabDesc()
{
    std::string first_unconsumed_line;

    std::unique_ptr<GameObjectStringDesc> prefab_serialized = LoadGameObject(first_unconsumed_line);
    FE_ASSERT(first_unconsumed_line == "", "All lines should be consumed");

    return prefab_serialized;
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

    std::unique_ptr<GameObjectStringDescProxy> child_desc = ParseChildByKey(line_trimmed.c_str(), m_File, m_CurrIndent, m_ReturnedLine);
    if (child_desc != nullptr)
    {
        m_GameObjectDesc->children.push_back(std::move(child_desc));
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