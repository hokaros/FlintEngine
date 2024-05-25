#pragma once
#include <Components/BoxCollider.h>
#include <ComponentDefinition.h>
#include <Core/GameObject.h>
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

protected:
	float speed = 1.0f;
	DECLARE_FIELD(speed);

	int damage = 1;
	DECLARE_FIELD(damage);

protected:
	virtual void OnCollision(BoxCollider& collider);

private:
	Vector direction = Direction::EAST;
	float currentLifeTime = 0.0f;

	static constexpr float s_MaxTimeToLive = 10.0f;
};

