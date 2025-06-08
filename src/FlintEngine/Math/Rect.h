#pragma once
#include <Math/Vector.h>
#include <Math/Segment.h>

struct Rect
{
public:
    // Corner of the rect with minimum x and y
    Vector pos;
    Vector size;
public:
    Rect(const Vector& position, const Vector& size);

    Vector GetMiddle() const;
    float CalculateArea() const;

    Segment GetSideHorizontalPositive() const;
    Segment GetSideHorizontalNegative() const;
    Segment GetSideVerticalPositive() const;
    Segment GetSideVerticalNegative() const;

    Vector GetCornerPositiveXPositiveY() const;
    Vector GetCornerNegativeXPositiveY() const;
    Vector GetCornerPositiveXNegativeY() const;
    Vector GetCornerNegativeXNegativeY() const;

    bool ContainsPoint(const Vector& point, float tolerance = 0.f) const;

    static Rect FromCenterAndSize(const Vector& center, const Vector& size);
};
