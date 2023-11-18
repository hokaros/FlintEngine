#pragma once
#include "../FlatEngine/BoxCollider.h"
#include "../FlatEngine/ComponentDefinition.h"
#include "Health.h"

// Obiekt biegn¹cy przed siebie do momentu kolizji
class Bullet :
	public ObjectComponent
{
	DECLARE_COMPONENT();

public:
	function<void(GameObject&, int dmg)> onPlayerCollision;

public:
	Bullet() = default;
	Bullet(float speed, int damage);
	void Awake() override;
	void Update() override;

	void SetDirection(const Vector& direction);

	virtual IUpdateable* Copy() override;

protected:
	float speed = 1.0f;
	int damage = 1;

protected:
	virtual void OnCollision(BoxCollider& collider);

private:
	Vector direction = Direction::EAST;

	DECLARE_FIELD(speed);
	DECLARE_FIELD(damage);
};

