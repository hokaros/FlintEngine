#pragma once
#include "Bullet.h"
// Liczba œcian, które mo¿e zniszczyæ jeden pocisk
#define POWER_BULLET_HEALTH 2

class PowerBullet 
	: public Bullet
{
public:
	PowerBullet(float speed, int damage);

	void Awake() override;
	virtual std::unique_ptr<ObjectComponent> Copy() override;

protected:
	virtual void OnCollision(BoxCollider& collider) override;

private:
	int health; // TODO: let's add this field (component inheritance)

private:
	void DecreaseHealth();
};

