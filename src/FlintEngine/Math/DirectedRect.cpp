#include "DirectedRect.h"

DirectedRect::DirectedRect(const Segment& mid, float width)
	: mid(mid)
	, width(width)
{
}

Vector DirectedRect::GetCorner1() const
{
	return mid.start + GetPerpendicularToMid() * width / 2.0f;
}

Vector DirectedRect::GetCorner2() const
{
	return mid.start - GetPerpendicularToMid() * width / 2.0f;
}

Vector DirectedRect::GetCorner3() const
{
	return mid.end + GetPerpendicularToMid() * width / 2.0f;
}

Vector DirectedRect::GetCorner4() const
{
	return mid.end - GetPerpendicularToMid() * width / 2.0f;
}

Segment DirectedRect::GetSide1() const
{
	const Vector side_half = GetPerpendicularToMid() * width;
	const Vector corner1 = mid.start + side_half;
	const Vector corner2 = mid.start - side_half;
	return Segment(corner1, corner2);
}

Segment DirectedRect::GetSide2() const
{
	const Vector side_half = GetPerpendicularToMid() * width;
	const Vector corner1 = mid.end + side_half;
	const Vector corner2 = mid.end - side_half;
	return Segment(corner1, corner2);
}

Segment DirectedRect::GetSide3() const
{
	const Vector side_half = GetPerpendicularToMid() * width;
	const Vector corner1 = mid.start + side_half;
	const Vector corner2 = mid.end + side_half;
	return Segment(corner1, corner2);
}

Segment DirectedRect::GetSide4() const
{
	const Vector side_half = GetPerpendicularToMid() * width;
	const Vector corner1 = mid.start - side_half;
	const Vector corner2 = mid.end - side_half;
	return Segment(corner1, corner2);
}

bool DirectedRect::DoesIntersect(const Segment& seg) const
{
	return seg.DoesCross(GetSide1())
		|| seg.DoesCross(GetSide2())
		|| seg.DoesCross(GetSide3())
		|| seg.DoesCross(GetSide4());
}

Vector DirectedRect::GetPerpendicularToMid() const
{
	const Vector mid_dir = mid.end - mid.start;
	return mid_dir.GetRotated(M_PI / 2.0f).GetNormalized();
}
