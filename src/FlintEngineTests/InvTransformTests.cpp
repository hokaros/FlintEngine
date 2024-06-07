#include "pch.h"
#include <Math/Transform.h>
#include "TestHelpers.h"

#ifdef SUITE_NAME
#error Cannot redefine suite name
#endif

#define SUITE_NAME InvTransform

TEST(SUITE_NAME, IdentityDoesNotChangePoint)
{
	// Arrange
	const Vector pos(0, 0);
	const Vector scale(1, 1);
	const Transform transform(pos, scale);

	const Vector in_point(5, -7.5f);

	// Act
	const Vector out_point = transform.InvTransformPoint(in_point);

	// Assert
	ASSERT_VEC_EQ(in_point, out_point);
}

TEST(SUITE_NAME, OffsetAppliesNegativeOffset)
{
	// Arrange
	const Vector pos = Vector(-0.5f, 2.7f);
	const Vector scale = Vector(1, 1);
	const Transform transform(pos, scale);

	const Vector in_point = Vector(5, -7.5f);

	// Act
	const Vector out_point = transform.InvTransformPoint(in_point);

	// Assert
	const Vector expected_point = in_point - pos;
	ASSERT_VEC_EQ(expected_point, out_point);
}

#undef SUITE_NAME