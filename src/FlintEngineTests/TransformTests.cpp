#include "pch.h"
#include <Math/Transform.h>

#ifdef SUITE_NAME
#error Cannot redefine suite name
#endif

#define SUITE_NAME Transform

constexpr float FLOAT_TOLERANCE = 0.001f;

#define ASSERT_VEC_EQ(expected, actual)		\
	ASSERT_FLOAT_EQ(expected.x, actual.x);	\
	ASSERT_FLOAT_EQ(expected.y, actual.y);

TEST(SUITE_NAME, IdentityDoesNotChangePoint)
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

TEST(SUITE_NAME, OffsetedAddsOffset)
{
	// Arrange
	const Vector pos(1, -2.5f);
	const Vector scale(1, 1);
	Transform transform(pos, scale);

	const Vector in_point(5, -7.5f);

	// Act
	const Vector out_point = transform.TransformPoint(in_point);

	// Assert
	const Vector expected_point = pos + in_point;
	ASSERT_VEC_EQ(expected_point, out_point);
}

TEST(SUITE_NAME, ScaledScalesOffset)
{
	// Arrange
	const Vector pos(0, 0);
	const Vector scale(0.5f, 2.7f);
	Transform transform(pos, scale);

	const Vector in_point(5, -7.5f);

	// Act
	const Vector out_point = transform.TransformPoint(in_point);

	// Assert
	const Vector expected_point = Vector(in_point.x * scale.x, in_point.y * scale.y);
	ASSERT_VEC_EQ(expected_point, out_point);
}

TEST(SUITE_NAME, ScaleAndOffsetScalesAndOffsets)
{
	// Arrange
	const Vector pos(1, -2.5f);
	const Vector scale(0.5f, 2.7f);
	Transform transform(pos, scale);

	const Vector in_point(5, -7.5f);

	// Act
	const Vector out_point = transform.TransformPoint(in_point);

	// Assert
	const float expected_x = in_point.x * scale.x + pos.x;
	const float expected_y = in_point.y * scale.y + pos.y;
	ASSERT_EQ(expected_x, out_point.x);
	ASSERT_EQ(expected_y, out_point.y);
}

#undef SUITE_NAME