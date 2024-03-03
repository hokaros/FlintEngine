#include "PrefabFactory.h"

#include "../FlatEngine/RectangleRenderer.h"
#include "../FlatEngine/AssetManager.h"
#include "../FlatEngine/SpriteRenderer.h"
#include "../LabyrinthShooterBase/PowerBullet.h"
#include "../LabyrinthShooterBase/Firearm.h"
#include "../LabyrinthShooterBase/ConstantMover.h"
#include "../LabyrinthShooterBase/PlayerEquipment.h"
#include "../LabyrinthShooterBase/PlayerController.h"

#include "../FlatEngine/GameObjectLoader.h"

#include "../FlatEngine/ComponentSerializer.h"
#include "../FlatEngine/GameObjectSerializer.h"

static constexpr const char* s_PlayerBitmapPath = "resources/player.bmp";

PrefabFactory::PrefabFactory()
{
	CreateBasicBulletPrefab();
	CreateSuperBulletPrefab();
	CreateBasicFirearmPrefab();
	CreateSuperFirearmPrefab();
	CreatePlayerPrefab();
}

const GameObject& PrefabFactory::GetPrefab(EPrefabId prefab_id) const
{
	return *(m_PrefabDict.find(prefab_id)->second);
}

void PrefabFactory::CreateBasicBulletPrefab()
{
	std::unique_ptr<GameObject> basic_bullet = GameObjectLoader::LoadPrefab("resources/basic_bullet.prefab");

	InsertPrefab(EPrefabId::BasicBullet, std::move(basic_bullet));
}

void PrefabFactory::CreateSuperBulletPrefab()
{
	std::unique_ptr<GameObject> super_bullet = GameObjectLoader::LoadPrefab("resources/power_bullet.prefab");

	InsertPrefab(EPrefabId::SuperBullet, std::move(super_bullet));
}

void PrefabFactory::CreateBasicFirearmPrefab()
{
	std::unique_ptr<GameObject> basic_weapon = GameObjectLoader::LoadPrefab("resources/basic_firearm.prefab");

	InsertPrefab(EPrefabId::BasicFirearm, std::move(basic_weapon));
}

void PrefabFactory::CreateSuperFirearmPrefab()
{
	std::unique_ptr<GameObject> super_weapon = GameObjectLoader::LoadPrefab("resources/super_firearm.prefab");

	InsertPrefab(EPrefabId::SuperFirearm, std::move(super_weapon));
}

void PrefabFactory::CreatePlayerPrefab()
{
	std::unique_ptr<GameObject> player = GameObjectLoader::LoadPrefab("resources/player.prefab");

	InsertPrefab(EPrefabId::Player, std::move(player));
}

void PrefabFactory::InsertPrefab(EPrefabId prefab_id, std::unique_ptr<GameObject> prefab)
{
	m_PrefabDict.insert({ prefab_id, std::move(prefab) });
}
