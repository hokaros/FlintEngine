#include "Triangle.h"

Triangle::Triangle(const Vector& v1, const Vector& v2, const Vector& v3)
	: v1(v1)
	, v2(v2)
	, v3(v3)
{
}

bool Triangle::Contains(const Vector& pos) const
{
	// Check if is on the same side of every line
	return false; // TODO
}
