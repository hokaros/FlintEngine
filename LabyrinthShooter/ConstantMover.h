#pragma once
#include "../FlatEngine/ObjectComponent.h"
#include "../FlatEngine/Vector.h"
#include <mutex>
#include <functional>

#define DIRECTION_TOLERANCE 0.001

class ConstantMover :
	public ObjectComponent
{
public:
	std::function<void(const Vector&)> onDirectionChanged;

public:
	ConstantMover(float movementSpeed);

	void Update() override;

	const Vector& GetDirection();
	void SetDirection(const Vector& newDir);
	// Czy obiekt porusza siê w okreœlon¹ stronê
	bool IsSameDirection(const Vector& otherDir) const;

	virtual std::unique_ptr<ObjectComponent> Copy() override;

protected:
	// Prêdkoœæ [piksele na sekundê]
	float movementSpeed;
	Vector moveDir;

private:
	std::mutex mutex;
};