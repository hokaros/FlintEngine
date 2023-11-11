#include "PrefabLoader.h"
#include <fstream>
#include "../FlatEngine/utility.h"

ComponentDefinitionTest PrefabLoader::s_Test;

std::unique_ptr<GameObject> PrefabLoader::LoadPrefab(const char* file_path)
{
    std::fstream prefab_file;
    prefab_file.open(file_path, std::ios::in);
    if (!prefab_file.is_open())
    {
        FE_LOG("Couldn't open prefab file: %s", file_path);
        return std::unique_ptr<GameObject>();
    }

    char line[256];
    while (!prefab_file.eof())
    {
        prefab_file.getline(line, 256);
        std::cout << "Loaded line: " << std::endl;
        std::cout << line << std::endl;
    }

    prefab_file.close();
    return std::unique_ptr<GameObject>();
}

ComponentDefinitionTest::ComponentDefinitionTest()
{
    std::cout << "Haha - component definition test successful" << std::endl;
}
