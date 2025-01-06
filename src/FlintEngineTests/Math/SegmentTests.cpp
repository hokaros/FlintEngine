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

TEST(SUITE_NAME, DoCrossOneHorizontalInverted)
{
	// Arrange
	const float x1 = 410.0f;
	const float x2 = 510.0f;
	const float y = 210.0f;
	const Segment horizontal_segment(Vector(x2, y), Vector(x1, y));

	const Segment other_segment(Vector(x1, 100.0f), Vector(x2, 300.0f));

	// Act
	const bool do_cross = horizontal_segment.DoesCross(other_segment);

	// Assert
	ASSERT_EQ(true, do_cross);
}

TEST(SUITE_NAME, DoCrossOneHorizontalInvertedOneDiagonalDown)
{
	// Arrange
	const Segment horizontal_segment(Vector(610, 110), Vector(510, 110));
	const Segment other_segment(Vector(510, 120), Vector(710, 20));

	// Act
	const bool do_cross = horizontal_segment.DoesCross(other_segment);
	const bool do_cross_inverted = other_segment.DoesCross(horizontal_segment);

	// Assert
	ASSERT_EQ(true, do_cross);
	ASSERT_EQ(true, do_cross_inverted);
}

TEST(SUITE_NAME, DoCrossOneHorizontalInvertedOneDiagonalDownShortened)
{
	// Arrange
	const float diff = 0.001f;
	const Segment horizontal_segment = Segment(Vector(610, 110), Vector(510, 110)).GetShortenedSegment(diff);
	const Segment other_segment = Segment(Vector(510, 120), Vector(710, 20)).GetShortenedSegment(diff);

	// Act
	const bool do_cross = horizontal_segment.DoesCross(other_segment);
	const bool do_cross_inverted = other_segment.DoesCross(horizontal_segment);

	// Assert
	ASSERT_EQ(true, do_cross);
	ASSERT_EQ(true, do_cross_inverted);
}

#undef SUITE_NAME