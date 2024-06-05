#include "Transform.h"

#define _USE_MATH_DEFINES
#include <math.h>

Transform::Transform(const Vector& position, const Vector& scale)
	: m_Position(position)
	, m_Rotation(0.f)
	, m_Scale(scale)
{
}

Transform::Transform(const Transform& other)
	: m_Position(other.m_Position)
	, m_Rotation(other.m_Rotation)
	, m_Scale(other.m_Scale)
{
}

const Vector& Transform::GetPosition() const
{
	return m_Position;
}

void Transform::SetPosition(const Vector& position)
{
	m_Position = position;
}

void Transform::Translate(const Vector& offset)
{
	m_Position += offset;
}

float Transform::GetRotation() const
{
	return m_Rotation;
}

void Transform::SetRotation(float rotation)
{
	m_Rotation = rotation;
}

void Transform::Rotate(float angle)
{
	m_Rotation += angle;
}

const Vector& Transform::GetScale() const
{
	return m_Scale;
}

void Transform::SetScale(const Vector& scale)
{
	m_Scale = scale;
}

Vector Transform::GetLookDir() const
{
	return Vector(
		cosf(m_Rotation * M_PI / 180.f),
		sinf(m_Rotation * M_PI / 180.f)
	);
}

Vector Transform::TransformPoint(const Vector& local_point) const
{ // TODO: let's test it
	Vector from_mid = local_point - m_Scale / 2.f;
	from_mid.Rotate(m_Rotation * M_PI / 180.f);

	Vector rotatedSize = m_Scale;
	rotatedSize.Rotate(m_Rotation * M_PI / 180.f);

	return from_mid + m_Position + rotatedSize / 2;
}
