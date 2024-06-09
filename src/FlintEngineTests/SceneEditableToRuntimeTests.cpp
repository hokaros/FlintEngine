#include "pch.h"
#include <Editables/EditableScene.h>
#include <Editables/InlineGameObject.h>
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
	EditableScene editable_scene;
	editable_scene.SetBackgroundColor(color);

	std::unique_ptr<Scene> runtime_scene = editable_scene.CreateRuntimeObject();

	// Assert
	ASSERT_EQ(color, runtime_scene->GetBackgroundColor());
}

TEST(SUITE_NAME, AddsSingleInlineObject)
{
	// Arrange
	const std::string object_name = "My unique inline game object";

	std::unique_ptr<InlineGameObject> game_object = std::make_unique<InlineGameObject>();
	game_object->Serializable_SetName(object_name);

	EditableScene editable_scene;
	editable_scene.AddRootObject(std::move(game_object));

	// Act
	std::unique_ptr<Scene> runtime_scene = editable_scene.CreateRuntimeObject();

	// Assert
	const std::list<std::unique_ptr<IGameObject>>& runtime_objects = runtime_scene->GetObjectManager().GetOwnedObjects();
	ASSERT_TRUE(runtime_objects.size() == 1);

	const std::unique_ptr<IGameObject>& runtime_object = runtime_objects.front();
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
	prefab_instance->Serializable_SetName(object_name);

	EditableScene editable_scene;
	editable_scene.AddRootObject(std::move(prefab_instance));

	// Act
	std::unique_ptr<Scene> runtime_scene = editable_scene.CreateRuntimeObject();

	// Assert
	const std::list<std::unique_ptr<IGameObject>>& runtime_objects = runtime_scene->GetObjectManager().GetOwnedObjects();
	ASSERT_TRUE(runtime_objects.size() == 1);

	const std::unique_ptr<IGameObject>& runtime_object = runtime_objects.front();
	ASSERT_EQ(object_name, runtime_object->GetName());
}

TEST(SUITE_NAME, Adds2InlineObjectsHierarchy)
{
	// Arrange
	const std::string root_object_name = "Good old root object";
	const std::string child_object_name = "Sweet child o' mine";

	std::unique_ptr<InlineGameObject> editable_root_object = std::make_unique<InlineGameObject>();
	editable_root_object->Serializable_SetName(root_object_name);

	std::unique_ptr<InlineGameObject> editable_child = std::make_unique<InlineGameObject>();
	editable_child->Serializable_SetName(child_object_name);
	editable_root_object->Serializable_AddChild(std::move(editable_child));

	EditableScene editable_scene;
	editable_scene.AddRootObject(std::move(editable_root_object));

	// Act
	std::unique_ptr<Scene> runtime_scene = editable_scene.CreateRuntimeObject();

	// Assert
	const std::list<std::unique_ptr<IGameObject>>& runtime_root_objects = runtime_scene->GetObjectManager().GetOwnedObjects();
	ASSERT_TRUE(runtime_root_objects.size() == 1);

	const std::unique_ptr<IGameObject>& runtime_root_object = runtime_root_objects.front();
	ASSERT_EQ(root_object_name, runtime_root_object->GetName());

	const std::vector<std::unique_ptr<IGameObject>>& runtime_root_object_children = runtime_root_object->GetChildren();
	ASSERT_TRUE(runtime_root_object_children.size() == 1);

	const std::unique_ptr<IGameObject>& runtime_child = runtime_root_object_children.front();
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

	std::unique_ptr<InlineGameObject> editable_root_object = std::make_unique<InlineGameObject>();
	editable_root_object->Serializable_SetName(root_object_name);

	std::unique_ptr<PrefabInstance> prefab_instance_child = std::make_unique<PrefabInstance>("irrelevant_path.prefab");
	prefab_instance_child->Serializable_SetName(child_object_name);
	editable_root_object->Serializable_AddChild(std::move(prefab_instance_child));

	EditableScene editable_scene;
	editable_scene.AddRootObject(std::move(editable_root_object));

	// Act
	std::unique_ptr<Scene> runtime_scene = editable_scene.CreateRuntimeObject();

	// Assert
	const std::list<std::unique_ptr<IGameObject>>& runtime_root_objects = runtime_scene->GetObjectManager().GetOwnedObjects();
	ASSERT_TRUE(runtime_root_objects.size() == 1);

	const std::unique_ptr<IGameObject>& runtime_root_object = runtime_root_objects.front();
	ASSERT_EQ(root_object_name, runtime_root_object->GetName());

	const std::vector<std::unique_ptr<IGameObject>>& runtime_root_object_children = runtime_root_object->GetChildren();
	ASSERT_TRUE(runtime_root_object_children.size() == 1);

	const std::unique_ptr<IGameObject>& runtime_child = runtime_root_object_children.front();
	ASSERT_EQ(child_object_name, runtime_child->GetName());
}


#undef SUITE_NAME