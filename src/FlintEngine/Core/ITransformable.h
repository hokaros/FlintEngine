#pragma once
#include <Math/Vector.h>

class ITransformable
{
public:
	// TODO: maybe we can just return const WorldTransform? 
	virtual const Vector& GetWorldPosition() const = 0;
	virtual void SetWorldPosition(const Vector& pos) = 0;
	virtual Vector GetLocalPosition() const = 0;
	virtual void SetLocalPosition(const Vector& pos) = 0;
	// Translate in world space
	virtual void Translate(const Vector& offset) = 0;

	virtual const Vector& GetWorldScale() const = 0;
	virtual void SetWorldScale(const Vector& scale) = 0;
	virtual Vector GetLocalScale() const = 0;
	virtual void SetLocalScale(const Vector& scale) = 0;

	virtual float GetWorldRotation() const = 0;
	virtual void SetWorldRotation(float rot) = 0;
	virtual float GetLocalRotation() const = 0;
	// Rotate in world space
	virtual void Rotate(float angle) = 0;
	// Rotates so that the x-axis of the object is heading towards specified world position
	virtual void LookAt(const Vector& pos) = 0;

	// Transforms from local space to world space
	virtual Vector TransformPoint(const Vector& local_pos) const = 0;
	// Transforms from world space to local space
	virtual Vector InvTransformPoint(const Vector& world_pos) const = 0;

public:
	Vector VectorLocalToWorld(const Vector& local_vector);
};