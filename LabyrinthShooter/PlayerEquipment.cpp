#include "PlayerEquipment.h"

PlayerEquipment::PlayerEquipment(GameObject& owner)
	: ObjectComponent(owner) {

}

void PlayerEquipment::Start() {
	LoadWeaponsFromChildren();

	EquipWeapon(basicWpn);

	if (superWpn != NULL) {
		superWpn->GetOwner().SetEnabled(false);
	}
}

ObjectComponent* PlayerEquipment::Copy(GameObject& newOwner) {
	return new PlayerEquipment(newOwner);
}

void PlayerEquipment::EquipWeapon(FirearmType weaponType) {
	if (weaponType == FirearmType::Basic) {
		EquipWeapon(basicWpn);
	}
	else {
		EquipWeapon(superWpn);
	}
}

void PlayerEquipment::SwitchWeapon() {
	if (currWpn == basicWpn)
		EquipWeapon(superWpn);
	else
		EquipWeapon(basicWpn);
}

void PlayerEquipment::EquipWeapon(Firearm* wpn) {
	if (currWpn != NULL) {
		currWpn->GetOwner().SetEnabled(false);
	}
	if (wpn != NULL) {
		wpn->GetOwner().SetEnabled(true);
	}

	currWpn = wpn;
}

Firearm* PlayerEquipment::GetCurrentWeapon() {
	if (currWpn == NULL)
		return NULL;

	return currWpn;
}

void PlayerEquipment::LoadWeaponsFromChildren() {
	std::list<Firearm*>* firearms = gameObject.FindComponentsInChildren<Firearm>();

	for (Firearm* firearm : *firearms) {
		if (firearm->GetType() == FirearmType::Basic) {
			basicWpn = firearm;
		}
		else {
			superWpn = firearm;
		}
	}

	delete firearms;
}