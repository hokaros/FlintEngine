#include "PrefabInstanceLoader.h"

PrefabInstanceLoader::PrefabInstanceLoader(std::fstream& file, size_t start_indent)
    : IndentFileParser<PrefabInstanceParsingState>(file, start_indent, PrefabInstanceParsingState::GeneralParams)
    , m_PrefabInstanceDesc(nullptr)
{
}

std::unique_ptr<PrefabInstanceStringDesc> PrefabInstanceLoader::LoadPrefabInstanceDesc()
{
    std::string first_unconsumed_line;

    std::unique_ptr<PrefabInstanceStringDesc> prefab_serialized = LoadPrefabInstanceDesc(first_unconsumed_line);
    FE_ASSERT(first_unconsumed_line == "", "All lines should be consumed");

    return prefab_serialized;
}

std::unique_ptr<PrefabInstanceStringDesc> PrefabInstanceLoader::LoadPrefabInstanceDesc(std::string& first_unconsumed_line)
{
    FE_ASSERT(m_PrefabInstanceDesc == nullptr, "Previous GameObject hasn't been moved");

    m_PrefabInstanceDesc = std::make_unique<PrefabInstanceStringDesc>();

    Load(first_unconsumed_line);

    FE_ASSERT(m_ParsingState == PrefabInstanceParsingState::GeneralParams, "Should have reached most outer parsing state");

    return std::move(m_PrefabInstanceDesc);
}

PrefabInstanceParsingState PrefabInstanceLoader::GetOuterParsingState(PrefabInstanceParsingState current_state)
{
    return PrefabInstanceParsingState::GeneralParams; // TODO
}

void PrefabInstanceLoader::ParseLineForCurrentState(const std::string& line)
{
    switch (m_ParsingState)
    {
    case PrefabInstanceParsingState::GeneralParams:
        ParseLineGeneralParams(line);
        break;

        // TODO
    }
}

void PrefabInstanceLoader::ParseLineGeneralParams(const std::string& line)
{
    std::string key, value;
    SplitLineToKeyAndValue(line, key, value);

    if (key == "Prefab")
    {
        m_PrefabInstanceDesc->m_PrefabPath = value;
    } // TODO: components
    else
    {
        m_PrefabInstanceDesc->m_OverridenParams.insert({ key, value });
    }
}
