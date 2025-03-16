#include "MathHelpers.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

float MathHelpers::NormalizeRadians(float rad)
{
	constexpr float full_circle = M_PI * 2.0f;

	return fmodf(rad, full_circle);
}

AngleQuarter MathHelpers::GetAngleQuarter(float rad)
{
	constexpr float first_quarter = M_PI / 2.0f;
	constexpr float second_quarter = M_PI;
	constexpr float third_quarter = 3.0f * M_PI / 2.0f;

	const float normalized_angle = abs(NormalizeRadians(rad));
	if (normalized_angle <= first_quarter)
		return AngleQuarter::First;
	else if (normalized_angle <= second_quarter)
		return AngleQuarter::Second;
	else if (normalized_angle <= third_quarter)
		return AngleQuarter::Third;
	else
		return AngleQuarter::Fourth;
}

bool MathHelpers::IsAngleFirstOrSecondQuarter(float rad)
{
	constexpr float second_quarter = M_PI;

	const float normalized_angle = abs(NormalizeRadians(rad));
	if (rad < 0.f)
		return IsAngleThirdOrFourthQuarter(normalized_angle);

	return normalized_angle <= second_quarter;
}

bool MathHelpers::IsAngleThirdOrFourthQuarter(float rad)
{
	constexpr float second_quarter = M_PI;

	const float normalized_angle = abs(NormalizeRadians(rad));
	if (rad < 0.f)
		return IsAngleFirstOrSecondQuarter(normalized_angle);

	return normalized_angle >= second_quarter || normalized_angle == 0.f;
}
