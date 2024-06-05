#pragma once
#include "Vector.h"

class Transform
{
public:
	Transform(const Vector& position, const Vector& scale);
	Transform(const Transform& other);

	const Vector& GetPosition() const;
	void SetPosition(const Vector& position);
	void Translate(const Vector& offset);

	float GetRotation() const;
	void SetRotation(float rotation);
	void Rotate(float angle);

	const Vector& GetScale() const;
	void SetScale(const Vector& scale);

	Vector GetLookDir() const;

	Vector TransformPoint(const Vector& local_point) const; // Transforms local position to outer space
	Vector InvTransformPoint(const Vector& outer_point) const; // Transforms position from outer to local space

private:
	Vector m_Position;
	float m_Rotation;
	Vector m_Scale;
};