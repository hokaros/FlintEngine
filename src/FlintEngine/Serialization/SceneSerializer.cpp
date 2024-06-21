#include "SceneSerializer.h"

std::unique_ptr<SceneStringDesc> SceneSerializer::Serialize(const EditableScene& scene)
{
    std::unique_ptr<SceneStringDesc> scene_serialized = std::make_unique<SceneStringDesc>();

    SerializeSceneParams(scene, *scene_serialized);
    SerializeGameObjects(scene, *scene_serialized);

    return scene_serialized;
}

std::unique_ptr<EditableScene> SceneSerializer::Deserialize(const SceneStringDesc& scene_desc)
{
    std::unique_ptr<EditableScene> scene_deserialized = std::make_unique<EditableScene>();

    DeserializeSceneParams(scene_desc, *scene_deserialized);
    DeserializeGameObjects(scene_desc, *scene_deserialized);

    return scene_deserialized;
}

void SceneSerializer::SerializeGameObjects(const EditableScene& scene, SceneStringDesc& scene_serialized)
{
    for (const std::unique_ptr<GameObject>& go : scene.GetRootObjects())
    {
        std::unique_ptr<GameObjectStringDescProxy> go_serialized = GameObjectSerializer::SerializeIEditable(*go);
        scene_serialized.game_objects.push_back(std::move(go_serialized));
    }
}

void SceneSerializer::SerializeSceneParams(const EditableScene& scene, SceneStringDesc& scene_serialized)
{
    scene_serialized.background_color = SerializableTypeInterface<Rgb8>::ToString(scene.GetBackgroundColor());
}

void SceneSerializer::DeserializeGameObjects(const SceneStringDesc& scene_serialized, EditableScene& scene)
{
    for (const std::unique_ptr<GameObjectStringDescProxy>& go_serialized : scene_serialized.game_objects)
    {
        std::unique_ptr<GameObject> go = GameObjectSerializer::DeserializeIEditable(*go_serialized);
        scene.AddRootObject(std::move(go));
    }
}

void SceneSerializer::DeserializeSceneParams(const SceneStringDesc& scene_serialized, EditableScene& scene)
{
    Rgb8 background_color;
    SerializableTypeInterface<Rgb8>::ParseString(scene_serialized.background_color, background_color);

    scene.SetBackgroundColor(background_color);
}
