#pragma once
#include "../FlatEngine/ComponentDefinition.h"
#include "../FlatEngine/Vector.h"
#include <mutex>
#include <functional>

#define DIRECTION_TOLERANCE 0.001

class ConstantMover :
	public ObjectComponent
{
	DECLARE_COMPONENT();
public:
	std::function<void(const Vector&)> onDirectionChanged;

public:
	ConstantMover() = default;
	ConstantMover(float movementSpeed);

	void Update() override;

	const Vector& GetDirection();
	void SetDirection(const Vector& newDir);
	// Czy obiekt porusza si� w okre�lon� stron�
	bool IsSameDirection(const Vector& otherDir) const;

	virtual std::unique_ptr<ObjectComponent> Copy() override;

protected:
	// Pr�dko�� [piksele na sekund�]
	float movementSpeed = 10.0f;
	Vector moveDir;

private:
	std::mutex mutex;
};