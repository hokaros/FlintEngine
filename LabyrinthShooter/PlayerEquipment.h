#pragma once
#include "../FlatEngine/ObjectComponent.h"
#include "Firearm.h"

// Komponent odpowiedzialny za zmienianie broni
class PlayerEquipment :
	public ObjectComponent
{
public:
	PlayerEquipment() = default;

	void Start() override;

	void EquipWeapon(FirearmType weaponType);
	void SwitchWeapon();

	Firearm* GetCurrentWeapon();

	virtual IUpdateable* Copy() override;

private:
	Firearm* basicWpn = NULL;
	Firearm* superWpn = NULL;

	Firearm* currWpn = NULL;

private:
	void EquipWeapon(Firearm* wpn);
	void LoadWeaponsFromChildren();
};

