#pragma once
#include "Bullet.h"
// Liczba �cian, kt�re mo�e zniszczy� jeden pocisk
#define POWER_BULLET_HEALTH 2

class PowerBullet :
	public Bullet
{
public:
	PowerBullet(float speed, int damage);

	void Awake() override;
	virtual IUpdateable* Copy() override;

protected:
	virtual void OnCollision(BoxCollider& collider) override;

private:
	int health;

private:
	void DecreaseHealth();
};

