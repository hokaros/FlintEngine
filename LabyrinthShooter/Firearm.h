#pragma once
#include "../FlatEngine/InputController.h"
#include "Bullet.h"
#include "Shared.h"

class Firearm :
	public ObjectComponent
{
public:
	function<void(GameObject&, int dmg)> onPlayerCollision; // zdarzenie, gdy pocisk trafi w gracza

public:
	Firearm(const GameObject& bulletPrefab, float reloadTime, FirearmType type);

	void Update() override;
	// Strzela, jeœli prze³adowano
	bool TryShoot();

	FirearmType GetType() const;

	virtual IUpdateable* Copy() override;

protected:
	const GameObject& bulletPrefab;
private:
	float reloadTime;
	float timeSinceLastShot = INFINITY;

	bool isReloaded = true; // czy gotowy do strza³u

	FirearmType type;
};

