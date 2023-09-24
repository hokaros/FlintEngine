#include "Vector.h"
#include <SDL_stdinc.h>

Vector::Vector()
	: x(0), y(0) {

}

Vector::Vector(float x, float y)
	: x(x), y(y) {

}

Vector::Vector(const VectorInt& other)
	: x(other.x), y(other.y) {

}

Vector::Vector(Direction direction) : x(0), y(0) {
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

void Vector::Normalize() {
	float length = Length();
	if (length == 0)
		return;

	x /= length;
	y /= length;
}

void Vector::Sort() {
	if (x <= y)
		return;

	float helper = x;
	x = y;
	y = x;
}

void Vector::Rotate(float angle) {
	float length = Length();

	float currAngle = GetAngle();
	float targetAngle = currAngle + angle;

	x = cos(targetAngle) * length;
	y = sin(targetAngle) * length;
}

double Vector::GetAngle() const {
	return atan2(y, x);
}

float Vector::Length() const {
	return sqrtf(x * x + y * y);
}

float Vector::LengthSquared() const {
	return x * x + y * y;
}


Vector operator+(const Vector& left, const Vector& right) {
	Vector result;
	result.x = left.x + right.x;
	result.y = left.y + right.y;

	return result;
}

Vector& operator+=(Vector& left, const Vector& right) {
	left.x += right.x;
	left.y += right.y;

	return left;
}

Vector operator-(const Vector& left, const Vector& right) {
	Vector result;
	result.x = left.x - right.x;
	result.y = left.y - right.y;

	return result;
}

Vector operator*(const Vector& left, float multiplier) {
	Vector result;
	result.x = left.x * multiplier;
	result.y = left.y * multiplier;

	return result;
}

Vector operator/(const Vector& left, float dividor) {
	Vector result;
	result.x = left.x / dividor;
	result.y = left.y / dividor;
	return result;
}

double Vector::GetAngle(const Vector& v1, const Vector& v2) {
	return v1.GetAngle() - v2.GetAngle();
}



VectorInt::VectorInt()
	: x(0), y(0) {

}

VectorInt::VectorInt(int x, int y)
	: x(x), y(y) {

}

VectorInt::VectorInt(const Vector& other)
	: x(other.x), y(other.y) {

}

VectorInt operator+(const VectorInt& left, const VectorInt& right) {
	VectorInt result;
	result.x = left.x + right.x;
	result.y = left.y + right.y;

	return result;
}

VectorInt& operator+=(VectorInt& left, const VectorInt& right) {
	left.x += right.x;
	left.y += right.y;

	return left;
}

VectorInt operator*(const VectorInt& left, int multiplier) {
	VectorInt result;
	result.x = left.x * multiplier;
	result.y = left.y * multiplier;

	return result;
}


Rect::Rect(const Vector& position, const Vector& size)
	: pos(position), size(size) {

}

Vector Rect::GetMiddle() const {
	return pos + size / 2;
}