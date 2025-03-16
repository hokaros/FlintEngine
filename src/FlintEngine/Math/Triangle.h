#pragma once
#include "Vector.h"

struct Triangle
{
public:
	Vector v1;
	Vector v2;
	Vector v3;

public:
	Triangle(const Vector& v1, const Vector& v2, const Vector& v3);
	bool Contains(const Vector& pos) const;
};