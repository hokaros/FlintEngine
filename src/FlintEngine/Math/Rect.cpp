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

Rect Rect::FromCenterAndSize(const Vector& center, const Vector& size)
{
	const Vector extents = size / 2.f;
	const Vector upper_left = center - extents;

	return Rect(upper_left, size);
}
