#include "SceneSerializer.h"

std::unique_ptr<SceneStringDesc> SceneSerializer::Serialize(const Scene& scene)
{
    std::unique_ptr<SceneStringDesc> scene_serialized = std::make_unique<SceneStringDesc>();

    SerializeSceneParams(scene, *scene_serialized);
    SerializeNavmeshGenerationParams(scene.GetNavmeshGenerationParams(), scene_serialized->navmesh_generation_params);
    SerializeGameObjects(scene, *scene_serialized);

    return scene_serialized;
}

std::unique_ptr<Scene> SceneSerializer::Deserialize(const SceneStringDesc& scene_desc)
{
    std::unique_ptr<Scene> scene_deserialized = std::make_unique<Scene>();

    DeserializeSceneParams(scene_desc, *scene_deserialized);
    DeserializeNavmeshGenerationParams(scene_desc.navmesh_generation_params, scene_deserialized->GetNavmeshGenerationParams());
    DeserializeGameObjects(scene_desc, *scene_deserialized);

    return scene_deserialized;
}

void SceneSerializer::SerializeGameObjects(const Scene& scene, SceneStringDesc& scene_serialized)
{
    for (const std::unique_ptr<GameObject>& go : scene.GetGameObjects())
    {
        std::unique_ptr<GameObjectStringDescProxy> go_serialized = GameObjectSerializer::SerializeIEditable(*go);
        scene_serialized.game_objects.push_back(std::move(go_serialized));
    }
}

void SceneSerializer::SerializeSceneParams(const Scene& scene, SceneStringDesc& scene_serialized)
{
    scene_serialized.background_color = SerializableTypeInterface<Rgb8>::ToString(scene.GetBackgroundColor());
}

void SceneSerializer::SerializeNavmeshGenerationParams(const Navigation::NavmeshGenerationParams& params, NavmeshGenerationParamsStringDesc& params_serialized)
{
    params_serialized.merge_distance = SerializableTypeInterface<float>::ToString(params.merge_distance);
}

void SceneSerializer::DeserializeGameObjects(const SceneStringDesc& scene_serialized, Scene& scene)
{
    for (const std::unique_ptr<GameObjectStringDescProxy>& go_serialized : scene_serialized.game_objects)
    {
        std::unique_ptr<GameObject> go = GameObjectSerializer::DeserializeIEditable(*go_serialized);
        scene.AddGameObject(std::move(go));
    }
}

void SceneSerializer::DeserializeSceneParams(const SceneStringDesc& scene_serialized, Scene& scene)
{
    Rgb8 background_color;
    SerializableTypeInterface<Rgb8>::ParseString(scene_serialized.background_color, background_color);

    scene.SetBackgroundColor(background_color);
}

void SceneSerializer::DeserializeNavmeshGenerationParams(const NavmeshGenerationParamsStringDesc& params_serialized, Navigation::NavmeshGenerationParams& params)
{
    if (!params_serialized.merge_distance.empty())
    {
        SerializableTypeInterface<float>::ParseString(params_serialized.merge_distance, params.merge_distance);
    }
    // Use default merge distance otherwise
}
