#include "pch.h"
#include <Core/Scene.h>
#include <Editables/PrefabInstance.h>

#include "FakeAssetManager.h"

#ifdef SUITE_NAME
	#error Cannot redefine suite name
#endif

#define SUITE_NAME SceneEditableToRuntime


TEST(SUITE_NAME, SetsBackgroundColor)
{
	// Arrange
	const Rgb8 color = Rgb8(0x01, 0x03, 0x02);

	// Act
	Scene editable_scene;
	editable_scene.SetBackgroundColor(color);

	std::unique_ptr<Scene> runtime_scene = std::make_unique<Scene>(editable_scene);

	// Assert
	ASSERT_EQ(color, runtime_scene->GetBackgroundColor());
}

TEST(SUITE_NAME, AddsSingleInlineObject)
{
	// Arrange
	const std::string object_name = "My unique inline game object";

	std::unique_ptr<GameObject> game_object = std::make_unique<GameObject>();
	game_object->SetName(object_name);

	Scene editable_scene;
	editable_scene.AddGameObject(std::move(game_object));

	// Act
	std::unique_ptr<Scene> runtime_scene = std::make_unique<Scene>(editable_scene);

	// Assert
	const std::vector<std::unique_ptr<GameObject>>& runtime_objects = runtime_scene->GetObjectManager().GetOwnedObjects();
	ASSERT_TRUE(runtime_objects.size() == 1);

	const std::unique_ptr<GameObject>& runtime_object = runtime_objects.front();
	ASSERT_EQ(object_name, runtime_object->GetName());
}

TEST(SUITE_NAME, AddsSinglePrefabInstance)
{
	// Arrange
	const std::string object_name = "My unique prefab instance";
	GameObject dummy_prefab;
	dummy_prefab.SetName("My holy prefab");

	FakeAssetManager asset_manager;
	asset_manager.SetPrefabToReturn(&dummy_prefab);

	std::unique_ptr<PrefabInstance> prefab_instance = std::make_unique<PrefabInstance>("irrelevant_path.prefab");
	prefab_instance->SetName(object_name);

	Scene editable_scene;
	editable_scene.AddGameObject(std::move(prefab_instance));

	// Act
	std::unique_ptr<Scene> runtime_scene = std::make_unique<Scene>(editable_scene);

	// Assert
	const Scene::GameObjectsT& runtime_objects = runtime_scene->GetGameObjects();
	ASSERT_TRUE(runtime_objects.size() == 1);

	const std::unique_ptr<GameObject>& runtime_object = runtime_objects.front();
	ASSERT_EQ(object_name, runtime_object->GetName());
}

TEST(SUITE_NAME, Adds2InlineObjectsHierarchy)
{
	// Arrange
	const std::string root_object_name = "Good old root object";
	const std::string child_object_name = "Sweet child o' mine";

	std::unique_ptr<GameObject> editable_root_object = std::make_unique<GameObject>();
	editable_root_object->SetName(root_object_name);

	std::unique_ptr<GameObject> editable_child = std::make_unique<GameObject>();
	editable_child->SetName(child_object_name);
	editable_root_object->AddChild(std::move(editable_child));

	Scene editable_scene;
	editable_scene.AddGameObject(std::move(editable_root_object));

	// Act
	std::unique_ptr<Scene> runtime_scene = std::make_unique<Scene>(editable_scene);

	// Assert
	const Scene::GameObjectsT& runtime_root_objects = runtime_scene->GetGameObjects();
	ASSERT_TRUE(runtime_root_objects.size() == 1);

	const std::unique_ptr<GameObject>& runtime_root_object = runtime_root_objects.front();
	ASSERT_EQ(root_object_name, runtime_root_object->GetName());

	const std::vector<std::unique_ptr<GameObject>>& runtime_root_object_children = runtime_root_object->GetChildren();
	ASSERT_TRUE(runtime_root_object_children.size() == 1);

	const std::unique_ptr<GameObject>& runtime_child = runtime_root_object_children.front();
	ASSERT_EQ(child_object_name, runtime_child->GetName());
}

TEST(SUITE_NAME, AddsInlineObjectAndPrefabInstanceChild)
{
	// Arrange
	const std::string root_object_name = "Good old root inline object";
	const std::string child_object_name = "Sweet instance o' thine";
	GameObject dummy_prefab;
	dummy_prefab.SetName("My holy prefab");
	FakeAssetManager asset_manager;
	asset_manager.SetPrefabToReturn(&dummy_prefab);

	std::unique_ptr<GameObject> editable_root_object = std::make_unique<GameObject>();
	editable_root_object->SetName(root_object_name);

	std::unique_ptr<PrefabInstance> prefab_instance_child = std::make_unique<PrefabInstance>("irrelevant_path.prefab");
	prefab_instance_child->SetName(child_object_name);
	editable_root_object->AddChild(std::move(prefab_instance_child));

	Scene editable_scene;
	editable_scene.AddGameObject(std::move(editable_root_object));

	// Act
	std::unique_ptr<Scene> runtime_scene = std::make_unique<Scene>(editable_scene);

	// Assert
	const Scene::GameObjectsT& runtime_root_objects = runtime_scene->GetGameObjects();
	ASSERT_TRUE(runtime_root_objects.size() == 1);

	const std::unique_ptr<GameObject>& runtime_root_object = runtime_root_objects.front();
	ASSERT_EQ(root_object_name, runtime_root_object->GetName());

	const std::vector<std::unique_ptr<GameObject>>& runtime_root_object_children = runtime_root_object->GetChildren();
	ASSERT_TRUE(runtime_root_object_children.size() == 1);

	const std::unique_ptr<GameObject>& runtime_child = runtime_root_object_children.front();
	ASSERT_EQ(child_object_name, runtime_child->GetName());
}


#undef SUITE_NAME