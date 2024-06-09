#include "pch.h"
#include <Editables/PrefabInstance.h>
#include <Components/RectangleRenderer.h>
#include <Components/BoxCollider.h>
#include "FakeAssetManager.h"

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
	FakeAssetManager asset_manager;
	asset_manager.SetPrefabToReturn(&dummy_prefab);

	std::unique_ptr<PrefabInstance> prefab_instance = std::make_unique<PrefabInstance>("irrelevant_path.prefab");

	// Act
	prefab_instance->Serializable_SetName(overriding_name);
	prefab_instance->Serializable_SetPosition(overriding_pos);
	prefab_instance->Serializable_SetSize(overriding_size);
	std::unique_ptr<GameObject> result_object = PrefabInstance::ToRuntimeObject(std::move(prefab_instance));

	// Assert
	ASSERT_EQ(overriding_name, result_object->GetName());
	ASSERT_EQ(overriding_pos, result_object->GetWorldPosition());
	ASSERT_EQ(overriding_size, result_object->GetWorldScale());
}

TEST(SUITE_NAME, KeepsNameAndPositionIfNotOverriden)
{
	// Arrange
	const std::string original_name = "My holy prefab";
	const Vector original_pos = Vector(10.5, -15);
	const Vector overriding_size = Vector(0.5, 2.3);

	GameObject dummy_prefab;
	dummy_prefab.SetName(original_name);
	dummy_prefab.SetLocalPosition(original_pos);
	FakeAssetManager asset_manager;
	asset_manager.SetPrefabToReturn(&dummy_prefab);

	std::unique_ptr<PrefabInstance> prefab_instance = std::make_unique<PrefabInstance>("irrelevant_path.prefab");

	// Act
	prefab_instance->Serializable_SetSize(overriding_size);
	std::unique_ptr<GameObject> result_object = PrefabInstance::ToRuntimeObject(std::move(prefab_instance));

	// Assert
	ASSERT_EQ(original_name, result_object->GetName());
	ASSERT_EQ(original_pos, result_object->GetWorldPosition());
	ASSERT_EQ(overriding_size, result_object->GetWorldScale());
}

TEST(SUITE_NAME, AddsComponent)
{
	// Arrange
	GameObject dummy_prefab;
	FakeAssetManager asset_manager;
	asset_manager.SetPrefabToReturn(&dummy_prefab);

	std::unique_ptr<PrefabInstance> prefab_instance = std::make_unique<PrefabInstance>("irrelevant_path.prefab");
	std::unique_ptr<RectangleRenderer> added_component = std::make_unique<RectangleRenderer>();

	// Act
	prefab_instance->Serializable_AddComponent(std::move(added_component));
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

	FakeAssetManager asset_manager;
	asset_manager.SetPrefabToReturn(&dummy_prefab);

	std::unique_ptr<PrefabInstance> prefab_instance = std::make_unique<PrefabInstance>("irrelevant_path.prefab");
	std::unique_ptr<RectangleRenderer> added_component = std::make_unique<RectangleRenderer>();

	// Act
	prefab_instance->Serializable_AddComponent(std::move(added_component));
	std::unique_ptr<GameObject> result_object = PrefabInstance::ToRuntimeObject(std::move(prefab_instance));

	// Assert
	RectangleRenderer* result_added_component = result_object->FindComponent<RectangleRenderer>();
	ASSERT_NE(nullptr, result_added_component);

	BoxCollider* result_original_component = result_object->FindComponent<BoxCollider>();
	ASSERT_NE(nullptr, result_original_component);
}

TEST(SUITE_NAME, ModifiesComponent)
{
	// Arrange
	const Vector original_size = Vector(10, 1);
	std::unique_ptr<BoxCollider> original_component = std::make_unique<BoxCollider>();
	original_component->SetSize(original_size);
	GameObject dummy_prefab;
	dummy_prefab.AddComponent(std::move(original_component));

	FakeAssetManager asset_manager;
	asset_manager.SetPrefabToReturn(&dummy_prefab);

	const Vector overriding_size = Vector(5, 3);
	std::unique_ptr<PrefabInstance> prefab_instance = std::make_unique<PrefabInstance>("irrelevant_path.prefab");

	// Act
	using FieldChangeT = ComponentFieldChangeContained<Vector>;
	std::unique_ptr<FieldChangeT> field_change = std::make_unique<FieldChangeT>();
	BoxCollider* instance_component = prefab_instance->GetResult().FindComponent<BoxCollider>();
	field_change->component = instance_component;
	field_change->field = &instance_component->GetFieldDefinition_m_Size();
	field_change->SetValue(overriding_size);
	prefab_instance->Serializable_ModifyComponentField(std::move(field_change));

	std::unique_ptr<GameObject> result_object = PrefabInstance::ToRuntimeObject(std::move(prefab_instance));

	// Assert
	BoxCollider* result_component = result_object->FindComponent<BoxCollider>();
	ASSERT_EQ(overriding_size, result_component->GetWorldSize());
}


#undef SUITE_NAME