#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

constexpr float DegToRad(float degrees)
{
    return degrees * M_PI / 180.f;
}

enum class Direction { NORTH, SOUTH, EAST, WEST };

struct Vector;

struct VectorInt
{
public:
    int x;
    int y;

public:
    constexpr VectorInt()
        : x(0)
        , y(0)
    {}

    constexpr VectorInt(int x, int y)
        : x(x)
        , y(y)
    {}

    VectorInt(const Vector& other);
    friend VectorInt operator+(const VectorInt& left, const VectorInt& right);
    friend VectorInt& operator+=(VectorInt& left, const VectorInt& right);
    friend VectorInt operator*(const VectorInt& left, int multiplier);
};

struct Vector
{
public:
    float x;
    float y;
public:
    constexpr Vector()
        : x(0)
        , y(0) 
    {}

    constexpr Vector(float x, float y)
        : x(x)
        , y(y) 
    {}

    constexpr Vector(const VectorInt& other)
        : x(other.x)
        , y(other.y) 
    {}

    constexpr Vector(const Vector& other)
        : x(other.x)
        , y(other.y)
    {}

    constexpr Vector(Direction direction) : x(0), y(0) {
        switch (direction) {
        case Direction::EAST:
            x = 1;
            y = 0;
            break;
        case Direction::WEST:
            x = -1;
            y = 0;
            break;
        case Direction::NORTH:
            x = 0;
            y = -1;
            break;
        case Direction::SOUTH:
            x = 0;
            y = 1;
            break;
        }
    }

    constexpr Vector(float values[2])
        : x(values[0])
        , y(values[1])
    {}

    // Sprowadza do d�ugo�ci 1
    Vector& Normalize();
    const Vector GetNormalized() const;

    // Sprawia, �e x <= y
    void Sort();
    float Length() const;
    float LengthSquared() const;
    // Obraca o k�t w radianach
    void Rotate(float angle);
    Vector GetRotated(float angle) const;
    Vector GetScaled(const Vector& other) const;
    // Zwraca k�t w radianach wskazywany przez wektor
    double GetAngle() const;

    float GetDistanceSq(const Vector& other) const;

    friend Vector operator+(const Vector& left, const Vector& right);
    friend Vector& operator+=(Vector& left, const Vector& right);
    friend Vector operator-(const Vector& left, const Vector& right);
    friend Vector operator-(const Vector& v);
    friend Vector operator*(const Vector& left, float multiplier);
    friend Vector& operator*=(Vector& left, float multiplier);
    friend Vector operator/(const Vector& left, float dividor);
    friend std::ostream& operator<<(std::ostream& ostr, const Vector& v);
    friend bool operator==(const Vector& left, const Vector& right);
    friend bool operator!=(const Vector& left, const Vector& right);

    // K�t w radianach mi�dzy dwoma wektorami
    static double GetAngle(const Vector& v1, const Vector& v2);
    // Component-wise multiplication
    static Vector Scale(const Vector& v1, const Vector& v2);
    static Vector Divide(const Vector& v1, const Vector& v2);
    static float Dot(const Vector& v1, const Vector& v2);

    static float GetDistance(const std::pair<Vector, Vector>& pair);
    static float GetDistanceSq(const std::pair<Vector, Vector>& pair);
    static float GetDistanceSq(const Vector& p1, const Vector& p2);

    // Sta�e
    static const Vector ZERO;
    static const Vector INVALID;
};

class VectorTreeComparator
{
public:
    bool operator()(const Vector& v1, const Vector& v2) const;
};