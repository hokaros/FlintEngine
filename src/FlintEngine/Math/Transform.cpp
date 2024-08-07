#include "Transform.h"

const Transform Transform::IDENTITY = Transform(Vector(0, 0), 0.0f, Vector(1, 1));

Transform::Transform(const Vector& position, const Vector& scale)
	: m_Position(position)
	, m_Rotation(0.f)
	, m_Scale(scale)
{
}

Transform::Transform(const Vector& position, float rotation, const Vector& scale)
	: m_Position(position)
	, m_Rotation(rotation)
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
{
	return local_point.GetScaled(m_Scale).GetRotated(m_Rotation * M_PI / 180.f) + m_Position;
}

Vector Transform::InvTransformPoint(const Vector& world_point) const
{
	const Vector inv_scale = Vector(1 / m_Scale.x, 1 / m_Scale.y);
	return (world_point - m_Position).GetRotated(-m_Rotation * M_PI / 180.f).GetScaled(inv_scale);
}
