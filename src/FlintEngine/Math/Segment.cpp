#include "Segment.h"

#include "Line.h"

bool Segment::DoesCross(const Segment& other) const
{
	const Vector infinite_line_crossing = Line::FromSegment(*this).GetCrossingPoint(Line::FromSegment(other));
	if (infinite_line_crossing == Vector::INVALID)
		return false;

	return IsPointBetweenEnds(infinite_line_crossing) && other.IsPointBetweenEnds(infinite_line_crossing);
}

bool Segment::IsPointBetweenEnds(const Vector& target) const
{
	const Vector from_p1_to_p2 = end - start;
	const Vector from_p1_to_target = target - start;
	if (abs(Vector::GetAngle(from_p1_to_p2, from_p1_to_target)) >= M_PI / 2.0)
		return false; // behind start

	const Vector from_p2_to_p1 = -from_p1_to_p2;
	const Vector from_p2_to_target = target - end;
	if (abs(Vector::GetAngle(from_p2_to_p1, from_p2_to_target)) >= M_PI / 2.0)
		return false; // behind end

	return true;
}
