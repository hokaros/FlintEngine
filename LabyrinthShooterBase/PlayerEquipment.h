#pragma once
#include "../FlatEngine/ComponentDefinition.h"
#include "Firearm.h"

// Komponent odpowiedzialny za zmienianie broni
class PlayerEquipment :
	public ObjectComponent
{
	DECLARE_COMPONENT();

public:
	PlayerEquipment() = default;

	void Start() override;

	void EquipWeapon(FirearmType weaponType);
	void SwitchWeapon();

	Firearm* GetCurrentWeapon();

	virtual std::unique_ptr<ObjectComponent> Copy() override;

private:
	Firearm* basicWpn = nullptr;
	Firearm* superWpn = nullptr;

	Firearm* currWpn = nullptr;

private:
	void EquipWeapon(Firearm* wpn);
	void LoadWeaponsFromChildren();
};

