#include "pch.h"
#include <Math/Transform.h>
#include <Core/GameObject.h>
#include "TestHelpers.h"

#ifdef SUITE_NAME
#error Cannot redefine suite name
#endif

#define SUITE_NAME TransformHierarchyTests

TEST(SUITE_NAME, ParentTranslationTranslatesChild)
{
	// Arrange
	const Vector parent_start_pos(-1, 2.5f);
	const Transform parent_transform(parent_start_pos, 0.f, Vector(1, 1));
	GameObject parent_object(parent_transform);

	parent_object.AddChild(std::make_unique<GameObject>(parent_transform));
	const GameObject& child_object = static_cast<const GameObject&>(*(parent_object.GetChildren()[0]));

	const Vector in_offset(7.2f, -0.5f);

	// Act
	parent_object.Translate(in_offset);

	// Assert
	const Vector expected_world_pos = parent_start_pos + in_offset;
	ASSERT_VEC_EQ(parent_object.GetWorldPosition(), child_object.GetWorldPosition());
	ASSERT_VEC_EQ(expected_world_pos, child_object.GetWorldPosition());
}

TEST(SUITE_NAME, ParentRotationRotatesAndTranslatesChild)
{
	// Arrange
	const Vector parent_start_pos = Vector(-1, 2.5f);
	const Transform parent_transform = Transform(parent_start_pos, 0.f, Vector(1, 1));
	GameObject parent_object = GameObject(parent_transform);

	const Vector child_offset = Vector(1.2f, 0.f);
	const Vector child_start_pos = parent_start_pos + child_offset;
	const Transform child_transform = Transform(child_start_pos, 0.f, Vector(1, 1));
	parent_object.AddChild(std::make_unique<GameObject>(child_transform));
	const GameObject& child_object = static_cast<const GameObject&>(*(parent_object.GetChildren()[0]));

	const float in_rotation = 45.f;

	// Act
	parent_object.Rotate(in_rotation);

	// Assert
	const Vector expected_child_pos = parent_start_pos + child_offset.GetRotated(DegToRad(in_rotation));
	ASSERT_VEC_EQ(expected_child_pos, child_object.GetWorldPosition());
	ASSERT_EQ(in_rotation, child_object.GetWorldRotation());
}

TEST(SUITE_NAME, SetLocalPositionDoesNotChangeRotationOrScale)
{
	// Arrange
	const Vector parent_pos = Vector(-1, 2.5f);
	const float parent_rotation = 45.f;
	const Vector parent_scale = Vector(1.25f, 3.7f);
	const Transform parent_transform = Transform(parent_pos, parent_rotation, parent_scale);

	const Vector child_pos = parent_transform.TransformPoint(Vector(2.f, 2.6f));
	const float child_start_rot = 27.f;
	const Vector child_start_scale = Vector(0.5f, 0.4f);
	const Transform child_transform = Transform(child_pos, child_start_rot, child_start_scale);

	GameObject parent_object = GameObject(parent_transform);
	parent_object.AddChild(std::make_unique<GameObject>(child_transform));
	GameObject& child_object = static_cast<GameObject&>(*(parent_object.GetChildren()[0]));

	// Act
	child_object.SetLocalPosition(Vector(-50.f, -50.f));

	// Assert
	ASSERT_EQ(child_start_rot, child_object.GetWorldRotation());
	ASSERT_VEC_EQ(child_start_scale, child_object.GetWorldScale());
}

#undef SUITE_NAME