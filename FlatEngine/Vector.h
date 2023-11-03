#pragma once
#include <math.h>
#include <iostream>

enum class Direction { NORTH, SOUTH, EAST, WEST };

struct Vector;

struct VectorInt
{
public:
    int x;
    int y;
public:
    VectorInt();
    VectorInt(int x, int y);
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

    Vector(Direction direction);

    // Sprowadza do d³ugoœci 1
    void Normalize();
    // Sprawia, ¿e x <= y
    void Sort();
    float Length() const;
    float LengthSquared() const;
    // Obraca o k¹t w radianach
    void Rotate(float angle);
    // Zwraca k¹t w radianach wskazywany przez wektor
    double GetAngle() const;

    friend Vector operator+(const Vector& left, const Vector& right);
    friend Vector& operator+=(Vector& left, const Vector& right);
    friend Vector operator-(const Vector& left, const Vector& right);
    friend Vector operator*(const Vector& left, float multiplier);
    friend Vector operator/(const Vector& left, float dividor);
    friend std::ostream& operator<<(std::ostream& ostr, const Vector& v);

    // K¹t w radianach miêdzy dwoma wektorami
    static double GetAngle(const Vector& v1, const Vector& v2);

    // Sta³e
    static const Vector ZERO;
};

struct Rect 
{
public:
    Vector pos;
    Vector size;
public:
    Rect(const Vector& position, const Vector& size);

    Vector GetMiddle() const;
    float CalculateArea() const;
};
