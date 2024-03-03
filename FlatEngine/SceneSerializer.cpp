#include "SceneSerializer.h"

std::unique_ptr<SceneStringDesc> SceneSerializer::Serialize(const Scene& scene)
{
    std::unique_ptr<SceneStringDesc> scene_serialized = std::make_unique<SceneStringDesc>();
    
    SerializeGameObjects(scene, *scene_serialized);
    SerializeSceneParams(scene, *scene_serialized);

    return scene_serialized;
}

std::unique_ptr<Scene> SceneSerializer::Deserialize(const SceneStringDesc& scene_desc)
{
    return std::unique_ptr<Scene>();
}

void SceneSerializer::SerializeGameObjects(const Scene& scene, SceneStringDesc& scene_serialized)
{
    for (const std::unique_ptr<GameObject>& go : scene.GetObjectManager().GetOwnedObjects())
    {
        std::unique_ptr<GameObjectStringDesc> go_serialized = GameObjectSerializer::SerializeGameObject(*go);
        scene_serialized.game_objects.push_back(std::move(go_serialized));
    }
}

void SceneSerializer::SerializeSceneParams(const Scene& scene, SceneStringDesc& scene_serialized)
{
    scene_serialized.background_color = SerializableTypeInterface<Rgb8>::ToString(scene.GetBackgroundColor());
}

void SceneSerializer::DeserializeGameObjects(const SceneStringDesc& scene_serialized, Scene& scene)
{
    for (const std::unique_ptr<GameObjectStringDesc>& go_serialized : scene_serialized.game_objects)
    {
        std::unique_ptr<GameObject> go = GameObjectSerializer::DeserializeGameObject(*go_serialized);
        scene.GetObjectManager().AddGameObject(std::move(go));
    }
}

void SceneSerializer::DeserializeSceneParams(const SceneStringDesc& scene_serialized, Scene& scene)
{
    Rgb8 background_color;
    SerializableTypeInterface<Rgb8>::ParseString(scene_serialized.background_color, background_color);

    scene.SetBackgroundColor(background_color);
}
