#include "PrefabFactory.h"

#include "../FlatEngine/RectangleRenderer.h"
#include "../FlatEngine/AssetManager.h"
#include "../FlatEngine/SpriteRenderer.h"
#include "../LabyrinthShooterBase/PowerBullet.h"
#include "../LabyrinthShooterBase/Firearm.h"
#include "../LabyrinthShooterBase/ConstantMover.h"
#include "../LabyrinthShooterBase/PlayerEquipment.h"
#include "../LabyrinthShooterBase/PlayerController.h"

#include "../FlatEngine/PrefabLoader.h"

#include "../FlatEngine/ComponentSerializer.h"
#include "../FlatEngine/GameObjectSerializer.h"

static constexpr const char* s_PlayerBitmapPath = "resources/player.bmp";
static constexpr const char* s_WeaponPrimaryBitmapPath = "resources/weapon_primary.bmp";
static constexpr const char* s_WeaponSuperBitmapPath = "resources/weapon_super.bmp";

PrefabFactory::PrefabFactory()
{
	LoadNeededAssets();

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

void PrefabFactory::LoadNeededAssets()
{
	AssetManager* asset_manager = AssetManager::GetInstance();

	asset_manager->AddAsset(s_PlayerBitmapPath);
	asset_manager->AddAsset(s_WeaponPrimaryBitmapPath);
	asset_manager->AddAsset(s_WeaponSuperBitmapPath);
}

void PrefabFactory::CreateBasicBulletPrefab()
{
	std::unique_ptr<GameObject> basic_bullet = PrefabLoader::LoadPrefab("resources/basic_bullet.prefab");

	InsertPrefab(EPrefabId::BasicBullet, std::move(basic_bullet));
}

void PrefabFactory::CreateSuperBulletPrefab()
{
	std::unique_ptr<GameObject> super_bullet = PrefabLoader::LoadPrefab("resources/power_bullet.prefab");

	InsertPrefab(EPrefabId::SuperBullet, std::move(super_bullet));
}

void PrefabFactory::CreateBasicFirearmPrefab()
{
	constexpr Vector basic_weapon_size = Vector(30, 10);

	std::unique_ptr<GameObject> basic_weapon = std::make_unique<GameObject>(basic_weapon_size, PrefabCreationKey());
	basic_weapon->AddComponent(std::make_unique<Firearm>(GetPrefab(EPrefabId::BasicBullet), WPN_BASIC_RELOAD, FirearmType::Basic));
	basic_weapon->AddComponent(std::make_unique<SpriteRenderer>(s_WeaponPrimaryBitmapPath));

	InsertPrefab(EPrefabId::BasicFirearm, std::move(basic_weapon));
}

void PrefabFactory::CreateSuperFirearmPrefab()
{
	constexpr Vector super_weapon_size = Vector(30, 10);

	std::unique_ptr<GameObject> super_weapon = std::make_unique<GameObject>(super_weapon_size, PrefabCreationKey());
	super_weapon->AddComponent(std::make_unique<Firearm>(GetPrefab(EPrefabId::SuperBullet), WPN_SUPER_RELOAD, FirearmType::Super));
	super_weapon->AddComponent(std::make_unique<SpriteRenderer>(s_WeaponSuperBitmapPath));

	InsertPrefab(EPrefabId::SuperFirearm, std::move(super_weapon));
}

void PrefabFactory::CreatePlayerPrefab()
{
	constexpr float player_speed = 300.0f;
	constexpr Vector player_size = Vector(20, 20);
	constexpr int player_max_health = 3;

	std::unique_ptr<GameObject> player = std::make_unique<GameObject>(player_size, PrefabCreationKey());

	player->AddComponent(std::make_unique<ConstantMover>(player_speed));
	player->AddComponent(std::make_unique<BoxCollider>(Vector::ZERO, player_size));
	player->AddComponent(std::make_unique<PlayerEquipment>());
	player->AddComponent(std::make_unique<Health>(player_max_health, nullptr));
	player->AddComponent(std::make_unique<PlayerController>());

	// Broñ
	const GameObject& basic_bullet = GetPrefab(PrefabFactory::EPrefabId::BasicBullet);
	const GameObject& super_bullet = GetPrefab(PrefabFactory::EPrefabId::SuperBullet);

	// Zwyk³a broñ
	GameObject* basic_weapon = new GameObject(
		GetPrefab(PrefabFactory::EPrefabId::BasicFirearm), {}
	);
	basic_weapon->SetPosition(player->GetPosition() + Vector(Direction::EAST) * player->GetSize().x);
	player->AddChild(basic_weapon);

	// Silna broñ
	GameObject* super_weapon = new GameObject(
		GetPrefab(PrefabFactory::EPrefabId::SuperFirearm), {}
	);
	super_weapon->SetPosition(player->GetPosition() + Vector(Direction::EAST) * player->GetSize().x);
	player->AddChild(super_weapon);

	player->AddComponent(std::make_unique<SpriteRenderer>(s_PlayerBitmapPath));

	InsertPrefab(EPrefabId::Player, std::move(player));
}

void PrefabFactory::InsertPrefab(EPrefabId prefab_id, std::unique_ptr<GameObject> prefab)
{
	m_PrefabDict.insert({ prefab_id, std::move(prefab) });
}
