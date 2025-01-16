#include "Segment.h"

#include "Line.h"

bool Segment::DoesCross(const Segment& other) const
{
	return GetCrossingPoint(other) != Vector::INVALID;
}

Vector Segment::GetCrossingPoint(const Segment& other) const
{
	const Vector infinite_line_crossing = Line::FromSegment(*this).GetCrossingPoint(Line::FromSegment(other));
	if (infinite_line_crossing == Vector::INVALID)
		return Vector::INVALID;

	if (IsPointBetweenEnds(infinite_line_crossing) && other.IsPointBetweenEnds(infinite_line_crossing))
	{
		return infinite_line_crossing;
	}
	else
	{
		return Vector::INVALID;
	}
}

const bool Segment::IsSameDirection(const Segment& other) const
{
	const Vector my_dir = (start - end).GetNormalized();
	const Vector other_dir = (other.start - other.end).GetNormalized();

	return my_dir == other_dir || my_dir == -other_dir;
}

Segment Segment::GetShortenedSegment(float diff_from_side) const
{
	const float diff_sum = diff_from_side * 2;
	if (diff_sum * diff_sum >= GetLengthSq())
		return Segment(GetMiddle(), GetMiddle());

	const Vector new_start = SlideInwardFromStart(diff_from_side);
	const Vector new_end = SlideInwardFromEnd(diff_from_side);

	return Segment(new_start, new_end);
}

float Segment::GetLength() const
{
	return (start - end).Length();
}

float Segment::GetLengthSq() const
{
	return (start - end).LengthSquared();
}

Vector Segment::GetMiddle() const
{
	return (start + end) / 2.0f;
}

bool Segment::IsPointBetweenEnds(const Vector& target) const
{
	const Vector from_p1_to_p2 = end - start;
	const Vector from_p1_to_target = target - start;
	const float start_angle = Vector::GetAngle(from_p1_to_p2, from_p1_to_target);
	if (!IsAngleFirstOrThirdQuarter(start_angle))
		return false; // behind start

	const Vector from_p2_to_p1 = -from_p1_to_p2;
	const Vector from_p2_to_target = target - end;
	const float end_angle = Vector::GetAngle(from_p2_to_p1, from_p2_to_target);
	if (!IsAngleFirstOrThirdQuarter(end_angle))
		return false; // behind end

	return true;
}

Vector Segment::SlideInwardFromStart(float dist) const
{
	const Vector from_start_to_end = end - start;
	const Vector diff_from_start = from_start_to_end.GetNormalized() * dist;
	return start + diff_from_start;
}

Vector Segment::SlideInwardFromEnd(float dist) const
{
	const Vector from_end_to_start = start - end;
	const Vector diff_from_end = from_end_to_start.GetNormalized() * dist;
	return end + diff_from_end;
}

float Segment::NormalizeRadians(float rad)
{
	constexpr float full_circle = M_PI * 2.0f;
	return fmodf(rad, full_circle);
}

bool Segment::IsAngleFirstOrThirdQuarter(float rad)
{
	constexpr float first_quarter = M_PI / 2.0f;
	constexpr float third_quarter = 3.0f * M_PI / 2.0f;

	const float normalized_angle = abs(NormalizeRadians(rad));
	return normalized_angle <= first_quarter || normalized_angle >= third_quarter;
}
