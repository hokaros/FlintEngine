#include "pch.h"
#include <Math/Segment.h>

#ifdef SUITE_NAME
#error Cannot redefine suite name
#endif

#define SUITE_NAME Segment

TEST(SUITE_NAME, DoCrossOneHorizontal)
{
	// Arrange
	const float x1 = 410.0f;
	const float x2 = 510.0f;
	const float y = 210.0f;
	const Segment horizontal_segment(Vector(x1, y), Vector(x2, y));

	const Segment other_segment(Vector(x1, 100.0f), Vector(x2, 300.0f));

	// Act
	const bool do_cross = horizontal_segment.DoesCross(other_segment);

	// Assert
	ASSERT_EQ(true, do_cross);
}

#undef SUITE_NAME