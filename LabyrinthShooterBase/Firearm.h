#pragma once
#include "../FlatEngine/InputController.h"
#include "Bullet.h"
#include "Shared.h"

class Firearm :
	public ObjectComponent
{
	DECLARE_COMPONENT();
public:
	function<void(GameObject&, int dmg)> onPlayerCollision; // zdarzenie, gdy pocisk trafi w gracza

public:
	Firearm() = default;
	Firearm(const GameObject& bulletPrefab, float reloadTime, FirearmType type);

	void Update() override;
	// Strzela, jeœli prze³adowano
	bool TryShoot();

	FirearmType GetType() const;

	virtual std::unique_ptr<ObjectComponent> Copy() override;

protected:
	const GameObject* bulletPrefab = nullptr;
private:
	float reloadTime = 1.0f;
	DECLARE_FIELD(reloadTime);

	FirearmType type = FirearmType::Basic; // TODO: make a field

	float timeSinceLastShot = INFINITY;

	bool isReloaded = true; // czy gotowy do strza³u
};

