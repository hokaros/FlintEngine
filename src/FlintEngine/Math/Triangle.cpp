#include "Triangle.h"

#include <Math/MathHelpers.h>

Triangle::Triangle(const Vector& v1, const Vector& v2, const Vector& v3)
	: v1(v1)
	, v2(v2)
	, v3(v3)
{
}

bool Triangle::Contains(const Vector& pos) const
{
	// Check if is on the same side of every edge
	const Vector v1_to_v2 = v2 - v1;
	const Vector v2_to_v3 = v3 - v2;
	const Vector v3_to_v1 = v1 - v3;

	const Vector v1_to_pos = pos - v1;
	const Vector v2_to_pos = pos - v2;
	const Vector v3_to_pos = pos - v3;
	
	bool all_on_left = true;
	bool all_on_right = true;
	auto check_vectors = [&all_on_right, &all_on_left](const Vector& lhs, const Vector& rhs)
	{
		const float angle = Vector::GetAngle(lhs, rhs);
		if (!MathHelpers::IsAngleFirstOrSecondQuarter(angle))
		{
			all_on_right = false;
		}
		else if(!MathHelpers::IsAngleThirdOrFourthQuarter(angle))
		{
			all_on_left = false;
		}
	};

	check_vectors(v1_to_v2, v1_to_pos);
	check_vectors(v2_to_v3, v2_to_pos);
	check_vectors(v3_to_v1, v3_to_pos);

	return all_on_right || all_on_left;
}
