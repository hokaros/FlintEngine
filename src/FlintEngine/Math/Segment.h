#pragma once
#include "Vector.h"
#include <vector>

struct Segment
{
public:
	Vector start;
	Vector end;

public:
	constexpr Segment(const Vector& start, const Vector& end)
		: start(start)
		, end(end)
	{}

	constexpr Segment(const std::pair<Vector, Vector>& ends)
		: start(ends.first)
		, end(ends.second)
	{}

	bool DoesCross(const Segment& other) const;
	Vector GetCrossingPoint(const Segment& other) const;

	const bool IsSameDirection(const Segment& other) const;

	/// <summary>
	/// Shortens the segment by the specified difference from each end
	/// </summary>
	Segment GetShortenedSegment(float diff_from_side) const;

	float GetLength() const;
	float GetLengthSq() const;
	Vector GetMiddle() const;

	std::vector<Segment> CutWith(const Segment& other, float tolerance = 0.001f) const;
	static std::vector<Segment> Cut(const Segment& s1, const Segment& s2, float tolerance = 0.001f);

	bool operator==(const Segment& other) const;

private:
	bool IsPointBetweenEnds(const Vector& point) const;

	Vector SlideInwardFromStart(float dist) const;
	Vector SlideInwardFromEnd(float dist) const;

	static float NormalizeRadians(float rad);
	static bool IsAngleFirstOrThirdQuarter(float rad);

	static bool TryGetEqualSegmentEnd(const Segment& segment, const Vector& desired_end_pos, Vector& end_pos);
};