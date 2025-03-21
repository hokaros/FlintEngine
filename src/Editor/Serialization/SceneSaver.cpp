#include "SceneSaver.h"

#include <Serialization/SceneLoader.h>
#include "PrefabSaver.h"

void SceneSaver::SaveScene(const Scene& scene, const char* file_path)
{
    std::fstream scene_file;
    scene_file.open(file_path, std::ios::out);
    if (!scene_file.is_open())
    {
        FE_LOG("Couldn't save prefab file: %s", file_path);
        return;
    }

    SceneSaver saver(scene_file);
    saver.SaveScene(scene);

    scene_file.close();
}

SceneSaver::SceneSaver(std::fstream& scene_file)
	: m_IndentPrinter(scene_file, 0)
{

}

void SceneSaver::SaveScene(const Scene& scene)
{
    std::unique_ptr<SceneStringDesc> scene_serialized = SceneSerializer::Serialize(scene);

    SaveSceneParameters(*scene_serialized);
    SaveSceneGameObjects(*scene_serialized);
}

void SceneSaver::SaveSceneParameters(const SceneStringDesc& scene)
{
    m_IndentPrinter.SaveKeyValuePair(SceneLoader::s_BackgroundColorKey, scene.background_color);
    SaveNavmeshGenerationParams(scene.navmesh_generation_params);
}

void SceneSaver::SaveNavmeshGenerationParams(const NavmeshGenerationParamsStringDesc& params)
{
    m_IndentPrinter.SaveKeyValuePair(SceneLoader::s_NavmeshMergeDistKey, params.merge_distance);
}

void SceneSaver::SaveSceneGameObjects(const SceneStringDesc& scene)
{
    m_IndentPrinter.SaveKeyValuePair(SceneLoader::s_GameObjectsKey, "");
    m_IndentPrinter.IncreaseIndent();

    for (const std::unique_ptr<GameObjectStringDescProxy>& game_object : scene.game_objects)
    {
        PrefabSaver prefab_saver = PrefabSaver(m_IndentPrinter);
        prefab_saver.SaveGameObject(*game_object);
    }
}
