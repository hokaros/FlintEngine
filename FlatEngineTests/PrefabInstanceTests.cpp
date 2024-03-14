#include "pch.h"
#include "../FlatEngine/PrefabInstance.h"
#include "../FlatEngine/RectangleRenderer.h"
#include "../FlatEngine/BoxCollider.h"

#ifdef SUITE_NAME
#error Cannot redefine suite name
#endif

#define SUITE_NAME PrefabInstance

TEST(SUITE_NAME, SetsNamePositionAndSize)
{
	// Arrange
	const std::string overriding_name = "This is the new way";
	const Vector overriding_pos = Vector(10.5, -15);
	const Vector overriding_size = Vector(0.5, 2.3);

	GameObject dummy_prefab;
	dummy_prefab.SetName("My holy prefab");

	std::unique_ptr<PrefabInstance> prefab_instance = std::make_unique<PrefabInstance>(dummy_prefab);

	// Act
	prefab_instance->SetName(overriding_name);
	prefab_instance->SetPosition(overriding_pos);
	prefab_instance->SetSize(overriding_size);
	std::unique_ptr<GameObject> result_object = PrefabInstance::ToRuntimeObject(std::move(prefab_instance));

	// Assert
	ASSERT_EQ(overriding_name, result_object->GetName());
	ASSERT_EQ(overriding_pos, result_object->GetPosition());
	ASSERT_EQ(overriding_size, result_object->GetSize());
}

TEST(SUITE_NAME, KeepsNameAndPositionIfNotOverriden)
{
	// Arrange
	const std::string original_name = "My holy prefab";
	const Vector original_pos = Vector(10.5, -15);
	const Vector overriding_size = Vector(0.5, 2.3);

	GameObject dummy_prefab;
	dummy_prefab.SetName(original_name);
	dummy_prefab.SetPosition(original_pos);

	std::unique_ptr<PrefabInstance> prefab_instance = std::make_unique<PrefabInstance>(dummy_prefab);

	// Act
	prefab_instance->SetSize(overriding_size);
	std::unique_ptr<GameObject> result_object = PrefabInstance::ToRuntimeObject(std::move(prefab_instance));

	// Assert
	ASSERT_EQ(original_name, result_object->GetName());
	ASSERT_EQ(original_pos, result_object->GetPosition());
	ASSERT_EQ(overriding_size, result_object->GetSize());
}

TEST(SUITE_NAME, AddsComponent)
{
	// Arrange
	GameObject dummy_prefab;
	std::unique_ptr<PrefabInstance> prefab_instance = std::make_unique<PrefabInstance>(dummy_prefab);
	std::unique_ptr<RectangleRenderer> added_component = std::make_unique<RectangleRenderer>();

	// Act
	prefab_instance->AddComponent(std::move(added_component));
	std::unique_ptr<GameObject> result_object = PrefabInstance::ToRuntimeObject(std::move(prefab_instance));

	// Assert
	RectangleRenderer* result_component = result_object->FindComponent<RectangleRenderer>();
	ASSERT_NE(nullptr, result_component);
}

TEST(SUITE_NAME, KeepsOriginalComponentsWhenAddingNew)
{
	// Arrange
	std::unique_ptr<BoxCollider> original_component = std::make_unique<BoxCollider>();
	GameObject dummy_prefab;
	dummy_prefab.AddComponent(std::move(original_component));

	std::unique_ptr<PrefabInstance> prefab_instance = std::make_unique<PrefabInstance>(dummy_prefab);
	std::unique_ptr<RectangleRenderer> added_component = std::make_unique<RectangleRenderer>();

	// Act
	prefab_instance->AddComponent(std::move(added_component));
	std::unique_ptr<GameObject> result_object = PrefabInstance::ToRuntimeObject(std::move(prefab_instance));

	// Assert
	RectangleRenderer* result_added_component = result_object->FindComponent<RectangleRenderer>();
	ASSERT_NE(nullptr, result_added_component);

	BoxCollider* result_original_component = result_object->FindComponent<BoxCollider>();
	ASSERT_NE(nullptr, result_original_component);
}


#undef SUITE_NAME