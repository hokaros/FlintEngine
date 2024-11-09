#include "Line.h"

Line Line::FromSegment(const Vector& p1, const Vector& p2)
{
	return Line(p1, p2 - p1);
}

Line Line::FromSegment(const std::pair<Vector, Vector>& points)
{
	return FromSegment(points.first, points.second);
}

Vector Line::GetCrossingPoint(const Line& other) const
{
	if (direction.x == 0.f || other.direction.x == 0.f)
	{ // Some line is vertical
		if (direction.x == 0.f && other.direction.x == 0.f)
		{ // Both lines are vertical
			if (anchor.x == other.anchor.x)
			{ // 2 vertical lines at the same x
				return anchor;
			}
			else
			{ // 2 vertical lines at different x
				return Vector::INVALID;
			}
		}
		else if (direction.x == 0.f)
		{ // Only this line is vertical
			const float this_x = anchor.x;
			return Vector(this_x, other.GetYAtX(this_x));
		}
		else
		{ // Only the other line is vertical
			const float other_x = other.anchor.x;
			return Vector(other_x, GetYAtX(other_x));
		}
	}
	else
	{ // No line is vertical
		const float a = GetSlopeFactor();
		const float other_a = other.GetSlopeFactor();
		const float b = GetYAtX(0.f);
		const float other_b = other.GetYAtX(0.f);

		if (a == other_a)
		{ // Parallel lines
			if (b == other_b)
			{ // These are the same lines
				return anchor;
			}
			else
			{
				return Vector::INVALID;
			}
		}
		else
		{
			const float cross_x = (other_b - b) / (a - other_a);
			return Vector(cross_x, GetYAtX(cross_x));
		}
	}
}

float Line::GetYAtX(float x) const
{
	const float dx = x - anchor.x;
	const float a = GetSlopeFactor();

	const float dy = a * dx;
	return anchor.y + dy;
}

float Line::GetSlopeFactor() const
{
	return direction.y / direction.x;
}
