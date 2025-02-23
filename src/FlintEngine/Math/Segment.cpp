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

std::vector<Segment> Segment::CutWith(const Segment& other) const
{
	return Cut(*this, other);
}

std::vector<Segment> Segment::Cut(const Segment& seg1, const Segment& seg2)
{
	std::vector<Segment> out_segments;
	out_segments.reserve(4); // 4 is the maximum number of segments after the operation

	if (seg1 == seg2)
	{
		out_segments.emplace_back(seg1);
		return out_segments;
	}

	auto return_input = [&out_segments, &seg1, &seg2]()
	{
		out_segments.clear();
		out_segments.emplace_back(seg1);
		out_segments.emplace_back(seg2);
		return out_segments;
	};

	if (seg1.GetLengthSq() == 0.f || seg2.GetLengthSq() == 0.f)
		return return_input();

	if (seg1.IsSameDirection(seg2))
		return return_input();

	const Vector crossing_point = seg1.GetCrossingPoint(seg2);
	if (crossing_point == Vector::INVALID)
		return return_input();

	Vector crossing_at_segment_end;
	const bool seg1_ends_at_crossing = TryGetEqualSegmentEnd(seg1, crossing_point, crossing_at_segment_end);
	const bool seg2_ends_at_crossing = TryGetEqualSegmentEnd(seg2, crossing_point, crossing_at_segment_end);
	if (seg1_ends_at_crossing && seg2_ends_at_crossing)
		return return_input();

	if (seg1_ends_at_crossing || seg2_ends_at_crossing)
	{ // One segment is not cut, there are no new vertices, the result are 3 segments with 1 mutual end
		auto return_3_segments = [&out_segments](const Vector& center, const Vector& end1, const Vector& end2, const Vector& end3)
		{
			out_segments.clear();
			out_segments.emplace_back(center, end1);
			out_segments.emplace_back(center, end2);
			out_segments.emplace_back(center, end3);
			return out_segments;
		};

		// Check the configuration in which to cut
		if (seg1.start == crossing_at_segment_end)
		{ // Cutting seg2 in the point of seg1.start
			return return_3_segments(seg1.start, seg1.end, seg2.start, seg2.end);
		}
		else if (seg1.end == crossing_at_segment_end)
		{ // Cutting seg2 in the point of seg1.end
			return return_3_segments(seg1.end, seg1.start, seg2.start, seg2.end);
		}
		else if (seg2.start == crossing_at_segment_end)
		{ // Cutting seg1 in the point of seg2.start
			return return_3_segments(seg2.start, seg2.end, seg1.start, seg1.end);
		}
		else
		{ // Cutting seg1 in the point of seg2.end
			return return_3_segments(seg2.end, seg2.start, seg1.start, seg1.end);
		}
	}
	else
	{ // The crossing point is between all ends
		// Cut all segments to the crossing point
		out_segments.clear();
		out_segments.emplace_back(crossing_point, seg1.start);
		out_segments.emplace_back(crossing_point, seg1.end);
		out_segments.emplace_back(crossing_point, seg2.start);
		out_segments.emplace_back(crossing_point, seg2.end);
		return out_segments;
	}
}

bool Segment::operator==(const Segment& other) const
{
	return (start == other.start || start == other.end)
		&& (end == other.start || end == other.end);
}

bool Segment::operator!=(const Segment& other) const
{
	return !(*this == other);
}

bool Segment::IsPointBetweenEnds(const Vector& target) const
{
	if (target == start || target == end)
		return true;

	const Vector from_p1_to_p2 = end - start;
	const Vector from_p1_to_target = target - start;
	const float start_angle = Vector::GetAngle(from_p1_to_p2, from_p1_to_target);
	if (IsAngleSecondOrThirdQuarter(start_angle))
		return false; // behind start

	const Vector from_p2_to_p1 = -from_p1_to_p2;
	const Vector from_p2_to_target = target - end;
	const float end_angle = Vector::GetAngle(from_p2_to_p1, from_p2_to_target);
	if (IsAngleSecondOrThirdQuarter(end_angle))
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

bool Segment::IsAngleFirstOrFourthQuarter(float rad)
{
	constexpr float first_quarter = M_PI / 2.0f;
	constexpr float third_quarter = 3.0f * M_PI / 2.0f;

	const float normalized_angle = abs(NormalizeRadians(rad));
	return normalized_angle <= first_quarter || normalized_angle >= third_quarter;
}

bool Segment::IsAngleSecondOrThirdQuarter(float rad)
{
	constexpr float first_quarter = M_PI / 2.0f;
	constexpr float third_quarter = 3.0f * M_PI / 2.0f;

	const float normalized_angle = abs(NormalizeRadians(rad));
	return normalized_angle >= first_quarter && normalized_angle <= third_quarter;
}

bool Segment::TryGetEqualSegmentEnd(const Segment& segment, const Vector& desired_end_pos, Vector& end_pos)
{
	if (segment.start == desired_end_pos)
	{
		end_pos = segment.start;
		return true;
	}
	if (segment.end == desired_end_pos)
	{
		end_pos = segment.end;
		return true;
	}

	return false;
}
