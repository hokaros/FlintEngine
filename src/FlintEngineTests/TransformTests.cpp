#include "pch.h"
#include <Math/Transform.h>

#ifdef SUITE_NAME
#error Cannot redefine suite name
#endif

#define SUITE_NAME Transform

constexpr float FLOAT_TOLERANCE = 0.001f;

#define ASSERT_VEC_EQ(expected, actual)	\
	ASSERT_TRUE((expected - actual).LengthSquared() < FLOAT_TOLERANCE*FLOAT_TOLERANCE);

TEST(SUITE_NAME, IdentityTransformDoesNotChangePoint)
{
	// Arrange
	const Vector pos(0, 0);
	const Vector scale(1, 1);
	Transform transform(pos, scale);

	const Vector in_point(5, -7.5f);

	// Act
	const Vector out_point = transform.TransformPoint(in_point);

	// Assert
	ASSERT_VEC_EQ(in_point, out_point);
}

#undef SUITE_NAME