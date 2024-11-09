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

private:
	bool IsPointBetweenEnds(const Vector& point) const;
};