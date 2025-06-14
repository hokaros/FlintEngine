#include "Rect.h"

Rect::Rect(const Vector& position, const Vector& size)
	: pos(position), size(size) {

}

Vector Rect::GetMiddle() const {
	return pos + size / 2;
}

float Rect::CalculateArea() const
{
	return size.x * size.y;
}

Segment Rect::GetSideHorizontalPositive() const
{
	return Segment(GetCornerNegativeXPositiveY(), GetCornerPositiveXPositiveY());
}

Segment Rect::GetSideHorizontalNegative() const
{
	return Segment(GetCornerNegativeXNegativeY(), GetCornerPositiveXNegativeY());
}

Segment Rect::GetSideVerticalPositive() const
{
	return Segment(GetCornerPositiveXNegativeY(), GetCornerPositiveXPositiveY());
}

Segment Rect::GetSideVerticalNegative() const
{
	return Segment(GetCornerNegativeXNegativeY(), GetCornerNegativeXPositiveY());
}

Vector Rect::GetCornerPositiveXPositiveY() const
{
	return pos + size;
}

Vector Rect::GetCornerNegativeXPositiveY() const
{
	return pos + Vector(0, size.y);
}

Vector Rect::GetCornerPositiveXNegativeY() const
{
	return pos + Vector(size.x, 0);
}

Vector Rect::GetCornerNegativeXNegativeY() const
{
	return pos;
}

bool Rect::ContainsPoint(const Vector& p, float tolerance) const
{
	const Vector rect_min = pos - Vector(tolerance / 2.0f, tolerance / 2.0f);
	const Vector rect_max = pos + size + Vector(tolerance / 2.0f, tolerance / 2.0f);

	return p.x >= rect_min.x && p.x <= rect_max.x
		&& p.y >= rect_min.y && p.y <= rect_max.y;
}

Rect Rect::FromCenterAndSize(const Vector& center, const Vector& size)
{
	const Vector extents = size / 2.f;
	const Vector upper_left = center - extents;

	return Rect(upper_left, size);
}
