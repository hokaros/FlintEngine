#pragma once
#include "../FlatEngine/BoxCollider.h"
#include "../FlatEngine/ComponentDefinition.h"
#include "../FlatEngine/GameObject.h"
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
	int damage = 1;

protected:
	virtual void OnCollision(BoxCollider& collider);

private:
	Vector direction = Direction::EAST;
	float currentLifeTime = 0.0f;

	static constexpr float s_MaxTimeToLive = 10.0f;

	DECLARE_FIELD(speed);
	DECLARE_FIELD(damage);
};

