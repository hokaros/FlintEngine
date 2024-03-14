#include "pch.h"
#include "../FlatEngine/EditableScene.h"
#include "../FlatEngine/InlineGameObject.h"
#include "../FlatEngine/PrefabInstance.h"

#ifdef SUITE_NAME
	#error Cannot redefine suite name
#endif

#define SUITE_NAME SceneEditableToRuntime


TEST(SUITE_NAME, SetsBackgroundColor)
{
	// Arrange
	const Rgb8 color = Rgb8(0x01, 0x03, 0x02);

	// Act
	EditableScene editable_scene;
	editable_scene.SetBackgroundColor(color);

	std::unique_ptr<Scene> runtime_scene = editable_scene.CreateRuntimeObject();

	// Assert
	ASSERT_EQ(color, runtime_scene->GetBackgroundColor());
}

TEST(SUITE_NAME, AddsSingleInlineObject)
{
	// Arrange
	std::string object_name = "My unique inline game object";

	// Act
	std::unique_ptr<InlineGameObject> game_object = std::make_unique<InlineGameObject>();
	game_object->SetName(object_name);

	EditableScene editable_scene;
	editable_scene.AddRootObject(std::move(game_object));

	std::unique_ptr<Scene> runtime_scene = editable_scene.CreateRuntimeObject();

	// Assert
	const std::list<std::unique_ptr<GameObject>>& runtime_objects = runtime_scene->GetObjectManager().GetOwnedObjects();
	ASSERT_TRUE(runtime_objects.size() == 1);

	const std::unique_ptr<GameObject>& runtime_object = runtime_objects.front();
	ASSERT_EQ(object_name, runtime_object->GetName());
}

TEST(SUITE_NAME, AddsSinglePrefabInstance)
{
	// Arrange
	std::string object_name = "My unique prefab instance";
	GameObject dummy_prefab;
	dummy_prefab.SetName("My holy prefab");

	// Act
	std::unique_ptr<PrefabInstance> prefab_instance = std::make_unique<PrefabInstance>(dummy_prefab);
	prefab_instance->SetName(object_name);

	EditableScene editable_scene;
	editable_scene.AddRootObject(std::move(prefab_instance));

	std::unique_ptr<Scene> runtime_scene = editable_scene.CreateRuntimeObject();

	// Assert
	const std::list<std::unique_ptr<GameObject>>& runtime_objects = runtime_scene->GetObjectManager().GetOwnedObjects();
	ASSERT_TRUE(runtime_objects.size() == 1);

	const std::unique_ptr<GameObject>& runtime_object = runtime_objects.front();
	ASSERT_EQ(object_name, runtime_object->GetName());
}

TEST(SUITE_NAME, Adds2InlineObjectsHierarchy)
{
	ASSERT_TRUE(false);
}

TEST(SUITE_NAME, AddsInlineObjectAndPrefabInstanceChild)
{
	ASSERT_TRUE(false);
}


#undef SUITE_NAME