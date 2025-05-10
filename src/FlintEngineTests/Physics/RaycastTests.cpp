#include "pch.h"
#include <PhysicsSystem.h>
#include <Core/GameObject.h>
#include <Components/BoxCollider.h>

#include <TestHelpers.h>
#include <UnitTestKeyCreator.h>

#ifdef SUITE_NAME
#error Cannot redefine suite name
#endif

#define SUITE_NAME Raycast

TEST(SUITE_NAME, CrossingColliderHorizontallyInMiddle)
{
	// Arrange
	PhysicsSystem physics(UnitTestKeyCreator::CreateSystemCreationKey());

	GameObject game_object;
	game_object.SetWorldPosition(Vector(10, 15));

	const Vector collider_offset = Vector::ZERO;
	const Vector collider_size = Vector(5, 6);
	game_object.AddComponent(std::make_unique<BoxCollider>(collider_offset, collider_size));

	// Act
	const Vector raycast_start = Vector(6, 15);
	const Vector raycast_end = Vector(15, 15);
	std::optional<RaycastHit> hit = physics.Raycast(raycast_start, raycast_end);

	// Assert
	ASSERT_TRUE(hit.has_value());
	
	ASSERT_VEC_EQ(Vector(7.5, 15), hit->collision_point);
}

#undef SUITE_NAME