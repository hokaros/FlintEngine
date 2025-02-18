#include "pch.h"
#include <Math/Segment.h>

#ifdef SUITE_NAME
#error Cannot redefine suite name
#endif

#define SUITE_NAME Segment

void assertAnySegmentEqual(const Segment& wanted_value, const std::vector<Segment>& actual_segments)
{
	for (const Segment& actual_segment : actual_segments)
	{
		if (actual_segment == wanted_value)
		{
			return;
		}
	}

	ASSERT_TRUE(false);
}

void assertEqualSegments(const std::vector<Segment>& expected_segments, const std::vector<Segment>& actual_segments)
{
	ASSERT_EQ(expected_segments.size(), actual_segments.size());

	for (const Segment& expected_seg : expected_segments)
	{
		assertAnySegmentEqual(expected_seg, actual_segments);
	}
}

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

TEST(SUITE_NAME, CutBetweenAt90deg)
{
	// Arrange
	const float horizontal_y = 20.0f;
	const Segment horizontal_segment = Segment(Vector(30.0f, horizontal_y), Vector(120.0f, horizontal_y));

	const float vertical_x = 60.0f;
	const Segment vertical_segment = Segment(Vector(vertical_x, 10.0f), Vector(vertical_x, 40.0f));

	// Act
	const std::vector<Segment> cut_result = horizontal_segment.CutWith(vertical_segment);

	// Assert
	std::vector<Segment> expected_result;
	expected_result.emplace_back(Segment(Vector(30, 20), Vector(60, 20)));
	expected_result.emplace_back(Segment(Vector(60, 20), Vector(120, 20)));
	expected_result.emplace_back(Segment(Vector(60, 10), Vector(60, 20)));
	expected_result.emplace_back(Segment(Vector(60, 20), Vector(60, 40)));

	assertEqualSegments(expected_result, cut_result);
}

TEST(SUITE_NAME, CutAtEndAt90deg)
{
	// Arrange
	const float horizontal_y = 40.0f;
	const Segment horizontal_segment = Segment(Vector(10, horizontal_y), Vector(120, horizontal_y));

	const float vertical_x = 120.0f;
	const Segment vertical_segment = Segment(Vector(vertical_x, 20), Vector(vertical_x, 60));

	// Act
	const std::vector<Segment> cut_result = horizontal_segment.CutWith(vertical_segment);

	// Assert
	std::vector<Segment> expected_result;
	expected_result.emplace_back(Segment(Vector(10, 40), Vector(120, 40)));
	expected_result.emplace_back(Segment(Vector(120, 20), Vector(120, 40)));
	expected_result.emplace_back(Segment(Vector(120, 40), Vector(120, 60)));

	assertEqualSegments(expected_result, cut_result);
}

#undef SUITE_NAME