#pragma once
#include "Bullet.h"

class PowerBullet 
	: public Bullet
{
	DECLARE_COMPONENT();
public:
	PowerBullet() = default;
	PowerBullet(float speed, int damage, int health);

	void Awake() override;

protected:
	virtual void OnCollision(BoxCollider& collider) override;

private:
	int health = 2;
	DECLARE_FIELD(health);

private:
	void DecreaseHealth();
};

