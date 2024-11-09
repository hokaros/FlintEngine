#pragma once
#include "Vector.h"

struct Line
{
public:
	Vector anchor;
	Vector direction;

public:
	constexpr Line(const Vector& anchor, const Vector& direction)
		: anchor(anchor)
		, direction(direction)
	{}

	static Line FromSegment(const Vector& p1, const Vector& p2);
	static Line FromSegment(const std::pair<Vector, Vector>& points);

	Vector GetCrossingPoint(const Line& other) const;
	float GetYAtX(float x) const;
	float GetSlopeFactor() const;
};