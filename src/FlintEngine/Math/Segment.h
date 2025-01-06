#pragma once
#include "Vector.h"

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

	/// <summary>
	/// Shortens the segment by the specified difference from each end
	/// </summary>
	Segment GetShortenedSegment(float diff_from_side) const;

	float GetLength() const;
	float GetLengthSq() const;
	Vector GetMiddle() const;

private:
	bool IsPointBetweenEnds(const Vector& point) const;

	Vector SlideInwardFromStart(float dist) const;
	Vector SlideInwardFromEnd(float dist) const;

	static float NormalizeRadians(float rad);
	static bool IsAngleFirstOrThirdQuarter(float rad);
};