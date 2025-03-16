#pragma once

enum class AngleQuarter
{
	First,
	Second,
	Third,
	Fourth
};


class MathHelpers
{
public:
	static float NormalizeRadians(float rad);
	static AngleQuarter GetAngleQuarter(float rad);

	static bool IsAngleFirstOrSecondQuarter(float rad);
	static bool IsAngleThirdOrFourthQuarter(float rad);
};