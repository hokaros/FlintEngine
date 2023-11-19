#include "PlayerEquipment.h"
#include "../FlatEngine/GameObject.h"

void PlayerEquipment::Start() 
{
	LoadWeaponsFromChildren();

	EquipWeapon(basicWpn);

	if (superWpn != nullptr) 
	{
		superWpn->GetOwner().SetEnabled(false);
	}
}

std::unique_ptr<ObjectComponent> PlayerEquipment::Copy()
{
	return std::make_unique<PlayerEquipment>();
}

void PlayerEquipment::EquipWeapon(FirearmType weaponType) 
{
	if (weaponType == FirearmType::Basic) 
	{
		EquipWeapon(basicWpn);
	}
	else 
	{
		EquipWeapon(superWpn);
	}
}

void PlayerEquipment::SwitchWeapon() 
{
	if (currWpn == basicWpn)
		EquipWeapon(superWpn);
	else
		EquipWeapon(basicWpn);
}

void PlayerEquipment::EquipWeapon(Firearm* wpn) 
{
	if (currWpn != nullptr) 
	{
		currWpn->GetOwner().SetEnabled(false);
	}
	if (wpn != nullptr) 
	{
		wpn->GetOwner().SetEnabled(true);
	}

	currWpn = wpn;
}

Firearm* PlayerEquipment::GetCurrentWeapon() 
{
	if (currWpn == nullptr)
		return nullptr;

	return currWpn;
}

void PlayerEquipment::LoadWeaponsFromChildren() 
{
	std::list<Firearm*>* firearms = m_GameObject->FindComponentsInChildren<Firearm>();

	for (Firearm* firearm : *firearms) 
	{
		if (firearm->GetType() == FirearmType::Basic) 
		{
			basicWpn = firearm;
		}
		else 
		{
			superWpn = firearm;
		}
	}

	delete firearms;
}