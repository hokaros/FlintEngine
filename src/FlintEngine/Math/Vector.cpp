#include "Vector.h"
#include <SDL_stdinc.h>

const Vector Vector::ZERO = Vector(0.0f, 0.0f);
const Vector Vector::INVALID = Vector(INFINITY, INFINITY);

Vector& Vector::Normalize() {
	float length = Length();
	if (length != 0)
	{
		x /= length;
		y /= length;
	}

	return *this;
}

const Vector Vector::GetNormalized() const {
	Vector result = Vector(*this);
	result.Normalize();

	return result;
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

Vector Vector::GetRotated(float angle) const {
	Vector result(*this);
	result.Rotate(angle);
	return result;
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

float Vector::GetDistanceSq(const Vector& other) const {
	return GetDistanceSq(*this, other);
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

Vector operator-(const Vector& v) {
	return Vector(-v.x, -v.y);
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

std::ostream& operator<<(std::ostream& ostr, const Vector& v)
{
	ostr << "(" << v.x << "; " << v.y << ")";
	return ostr;
}

Vector& operator*=(Vector& left, float multiplier)
{
	left.x *= multiplier;
	left.y *= multiplier;

	return left;
}

bool operator==(const Vector& left, const Vector& right)
{
	return left.x == right.x && left.y == right.y;
}

bool operator!=(const Vector& left, const Vector& right)
{
	return !(left == right);
}

double Vector::GetAngle(const Vector& v1, const Vector& v2)
{
	return v1.GetAngle() - v2.GetAngle();
}

Vector Vector::Scale(const Vector& v1, const Vector& v2)
{
	return Vector(v1.x * v2.x, v1.y * v2.y);
}

Vector Vector::GetScaled(const Vector& other) const
{
	return Scale(*this, other);
}

Vector Vector::Divide(const Vector& v1, const Vector& v2)
{
	return Vector(v1.x / v2.x, v1.y / v2.y);
}

float Vector::Dot(const Vector& v1, const Vector& v2)
{
	return v1.Length() * v2.Length() * cos(GetAngle(v1, v2));
}

float Vector::GetDistance(const std::pair<Vector, Vector>& pair)
{
	return (pair.first - pair.second).Length();
}

float Vector::GetDistanceSq(const std::pair<Vector, Vector>& pair)
{
	return GetDistanceSq(pair.first, pair.second);
}

float Vector::GetDistanceSq(const Vector& p1, const Vector& p2)
{
	return (p1 - p2).LengthSquared();
}


bool VectorTreeComparator::operator()(const Vector& v1, const Vector& v2) const
{
	if (v1.x < v2.x)
		return true;

	if (v1.x > v2.x)
		return false;

	return v1.y < v2.y;
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

float Rect::CalculateArea() const
{
	return size.x * size.y;
}

Rect Rect::FromCenterAndSize(const Vector& center, const Vector& size)
{
	const Vector extents = size / 2.f;
	const Vector upper_left = center - extents;

	return Rect(upper_left, size);
}
