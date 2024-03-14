#include "PrefabInstanceLoader.h"

PrefabInstanceLoader::PrefabInstanceLoader(std::fstream& file, size_t start_indent)
    : IndentFileParser<PrefabInstanceParsingState>(file, start_indent, PrefabInstanceParsingState::GameObjectParams)
{
}

std::unique_ptr<PrefabInstanceStringDesc> PrefabInstanceLoader::LoadPrefabInstanceDesc()
{
    return std::unique_ptr<PrefabInstanceStringDesc>(); // TODO
}

std::unique_ptr<PrefabInstanceStringDesc> PrefabInstanceLoader::LoadPrefabInstanceDesc(std::string& first_unconsumed_line)
{
    return std::unique_ptr<PrefabInstanceStringDesc>(); // TODO
}

PrefabInstanceParsingState PrefabInstanceLoader::GetOuterParsingState(PrefabInstanceParsingState current_state)
{
    return PrefabInstanceParsingState::GameObjectParams; // TODO
}

void PrefabInstanceLoader::ParseLineForCurrentState(const std::string& line)
{
    // TODO
}
