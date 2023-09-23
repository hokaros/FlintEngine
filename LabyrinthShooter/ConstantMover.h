#pragma once
#include "ObjectComponent.h"
#include <mutex>

#define DIRECTION_TOLERANCE 0.001

class ConstantMover :
	public ObjectComponent
{
public:
	function<void(const Vector&)> onDirectionChanged;

public:
	ConstantMover(GameObject& owner, float movementSpeed);

	void Update() override;

	const Vector& GetDirection();
	void SetDirection(const Vector& newDir);
	// Czy obiekt porusza siê w okreœlon¹ stronê
	bool IsSameDirection(const Vector& otherDir) const;

	ObjectComponent* Copy(GameObject& newOwner) override;

protected:
	// Prêdkoœæ [piksele na sekundê]
	float movementSpeed;
	Vector moveDir;

private:
	std::mutex mutex;
};