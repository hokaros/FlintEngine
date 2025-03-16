#include "pch.h"
#include <Math/Triangle.h>

#ifdef SUITE_NAME
#error Cannot redefine suite name
#endif

#define SUITE_NAME Triangle

TEST(SUITE_NAME, NotContainsOutsideFar)
{
	// Arrange
	const Triangle tri = Triangle(Vector(1, 1), Vector(3, 1), Vector(2, 3));
	const Vector tested_point = Vector(-10, -10);

	// Act
	const bool contains_point = tri.Contains(tested_point);

	// Assert
	ASSERT_EQ(false, contains_point);
}

TEST(SUITE_NAME, NotContainsOutsideNear)
{
	// Arrange
	const Triangle tri = Triangle(Vector(1, 1), Vector(3, 1), Vector(2, 3));
	const Vector tested_point = Vector(3, 2.9f);

	// Act
	const bool contains_point = tri.Contains(tested_point);

	// Assert
	ASSERT_EQ(false, contains_point);
}

TEST(SUITE_NAME, ContainsCenter)
{
	// Arrange
	const Triangle tri = Triangle(Vector(1, 1), Vector(3, 1), Vector(2, 3));
	const Vector tested_point = Vector(2, 2);

	// Act
	const bool contains_point = tri.Contains(tested_point);

	// Assert
	ASSERT_EQ(true, contains_point);
}

TEST(SUITE_NAME, ContainsNearSide)
{
	// Arrange
	const Triangle tri = Triangle(Vector(1, 1), Vector(3, 1), Vector(1, 3));
	const Vector tested_point = Vector(1.1f, 2.5f);

	// Act
	const bool contains_point = tri.Contains(tested_point);

	// Assert
	ASSERT_EQ(true, contains_point);
}

#undef SUITE_NAME