#pragma once
#include "../FlatEngine/ObjectComponent.h"
#include <mutex>

#define DIRECTION_TOLERANCE 0.001

class ConstantMover :
	public ObjectComponent
{
public:
	function<void(const Vector&)> onDirectionChanged;

public:
	ConstantMover(float movementSpeed);

	void Update() override;

	const Vector& GetDirection();
	void SetDirection(const Vector& newDir);
	// Czy obiekt porusza si� w okre�lon� stron�
	bool IsSameDirection(const Vector& otherDir) const;

	virtual IUpdateable* Copy() override;

protected:
	// Pr�dko�� [piksele na sekund�]
	float movementSpeed;
	Vector moveDir;

private:
	std::mutex mutex;
};