#pragma once
#include <Math/Vector.h>

struct Rect
{
public:
    Vector pos;
    Vector size;
public:
    Rect(const Vector& position, const Vector& size);

    Vector GetMiddle() const;
    float CalculateArea() const;

    static Rect FromCenterAndSize(const Vector& center, const Vector& size);
};
