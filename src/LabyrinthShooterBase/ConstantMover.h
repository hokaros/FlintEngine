#pragma once
#include <mutex>
#include <functional>
#include <ComponentDefinition.h>
#include <Math/Vector.h>
#include <Navigation/IMovable.h>

#define DIRECTION_TOLERANCE 0.001

class ConstantMover
	: public ObjectComponent
	, public IMovable
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
	// Czy obiekt porusza siê w okreœlon¹ stronê
	bool IsSameDirection(const Vector& otherDir) const;

protected:
	// IMovable
	virtual void MoveTowards(const Vector& pos) override;
	virtual void Stop() override;
	virtual Vector GetPosition() const override;

protected:
	// Prêdkoœæ [piksele na sekundê]
	float movementSpeed = 10.0f;
	DECLARE_FIELD(movementSpeed);

	Vector moveDir;

private:
	std::mutex mutex;
};