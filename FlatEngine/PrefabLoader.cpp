#include "PrefabLoader.h"
#include <fstream>
#include <string>
#include "../FlatEngine/utility.h"
#include "../FlatEngine/GameObjectSerializer.h"

GameObject* PrefabLoader::LoadPrefab(const char* file_path)
{
    std::fstream prefab_file;
    prefab_file.open(file_path, std::ios::in);
    if (!prefab_file.is_open())
    {
        FE_LOG("Couldn't open prefab file: %s", file_path);
        return nullptr;
    }

    PrefabLoader prefab_loader;
    GameObject* prefab = prefab_loader.LoadPrefab(prefab_file);

    prefab_file.close();
    return prefab;
}

GameObject* PrefabLoader::LoadPrefab(std::fstream& file)
{
    char line[256];
    while (!file.eof())
    {
        file.getline(line, 256);
        std::cout << "Loaded line: " << std::endl;
        std::cout << line << std::endl;
    }

    ParseGameObjectParamLine("size: 4, 4");
    ParseGameObjectParamLine("components:");

    {
        ParseComponentNameLine("Bullet:");
        ParseComponentFieldLine("speed: 1000");
        ParseComponentFieldLine("damage: 1");
    }

    {
        ParseComponentNameLine("BoxCollider:");
        ParseComponentFieldLine("m_Pos: 0, 0");
        ParseComponentFieldLine("m_Size: 4, 4");
    }

    {
        ParseComponentNameLine("RectangleRenderer:");
        ParseComponentFieldLine("m_Color: 0xFF, 0xFF, 0x00");
    }

    SetParsingState(ParsingState::GameObjectParams);

    return GameObjectSerializer::DeserializeGameObject(m_GameObjectDesc);
}

void PrefabLoader::ParseGameObjectParamLine(const std::string& line)
{
    std::string key, value;
    SplitLineToKeyAndValue(line, key, value);
    
    if (key == "components")
    {
        FE_ASSERT(value.size() == 0, "Inline value for components detected");

        SetParsingState(ParsingState::ComponentDefinitions);
    }
    else
    {
        m_GameObjectDesc.params.insert({ key, value });
    }
}

void PrefabLoader::ParseComponentNameLine(const std::string& line)
{
    SetParsingState(ParsingState::SpecificComponentDefinition);

    std::string key, value;
    SplitLineToKeyAndValue(line, key, value);
    FE_ASSERT(value.size() == 0, "Value after component name detected");

    m_CurrComponentDesc.type = key;
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
    if (m_ParsingState == ParsingState::SpecificComponentDefinition)
    {
        FinalizeComponentLoading();
    }

    m_ParsingState = state;
}

void PrefabLoader::SplitLineToKeyAndValue(const std::string& line, std::string& key, std::string& value)
{
    size_t colon_pos = line.find(':');
    FE_ASSERT(colon_pos != (size_t)-1, "Colon not found");

    key = line.substr(0, colon_pos);
    TrimWhitespaces(key);
    if (colon_pos == line.size() - 1)
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
