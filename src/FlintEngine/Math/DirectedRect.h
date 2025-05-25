#pragma once
#include <Math/Vector.h>
#include <Math/Segment.h>

struct DirectedRect
{
public:
	Segment mid;
	float width;

public:
	DirectedRect() = default;
	DirectedRect(const Segment& mid, float width);

	Vector GetCorner1() const;
	Vector GetCorner2() const;
	Vector GetCorner3() const;
	Vector GetCorner4() const;

	Segment GetSide1() const;
	Segment GetSide2() const;
	Segment GetSide3() const;
	Segment GetSide4() const;

	bool DoesIntersect(const Segment& seg) const;

private:
	Vector GetPerpendicularToMid() const;
};