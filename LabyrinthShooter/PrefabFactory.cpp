#include "PrefabFactory.h"

#include "../FlatEngine/RectangleRenderer.h"
#include "../FlatEngine/AssetManager.h"
#include "../FlatEngine/SpriteRenderer.h"
#include "PowerBullet.h"
#include "Firearm.h"
#include "ConstantMover.h"
#include "PlayerEquipment.h"
#include "PlayerController.h"

#include "../FlatEngine/PrefabLoader.h"

#include "../FlatEngine/ComponentSerializer.h"
#include "../FlatEngine/GameObjectSerializer.h"

static constexpr const char* s_PlayerBitmapPath = "resources/player.bmp";
static constexpr const char* s_WeaponPrimaryBitmapPath = "resources/weapon_primary.bmp";
static constexpr const char* s_WeaponSuperBitmapPath = "resources/weapon_super.bmp";
static constexpr const char* s_HeartBitmapPath = "resources/heart.bmp";

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
	asset_manager->AddAsset(s_HeartBitmapPath);
}

void PrefabFactory::CreateBasicBulletPrefab()
{
	std::unique_ptr<GameObject> basic_bullet = PrefabLoader::LoadPrefab("resources/basic_bullet.prefab");

	InsertPrefab(EPrefabId::BasicBullet, std::move(basic_bullet));
}

void PrefabFactory::CreateSuperBulletPrefab()
{
	constexpr Vector super_bullet_size = Vector(10, 10);
	Rgb8 super_bullet_color = Rgb8(0xFF, 0x00, 0x00);

	std::unique_ptr<GameObject> super_bullet = std::make_unique<GameObject>(super_bullet_size, PrefabCreationKey());

	super_bullet->AddComponent(std::make_unique<PowerBullet>(BULLET_SUPER_SPEED, BULLET_SUPER_DAMAGE));
	super_bullet->AddComponent(std::make_unique<BoxCollider>(Vector::ZERO, super_bullet_size));
	super_bullet->AddComponent(std::make_unique<RectangleRenderer>(super_bullet_color));

	InsertPrefab(EPrefabId::SuperBullet, std::move(super_bullet));
}

void PrefabFactory::CreateBasicFirearmPrefab()
{
	constexpr Vector basic_weapon_size = Vector(30, 10);
	SDL_Surface* bitmap = AssetManager::GetInstance()->GetSurfaceAsset(s_WeaponPrimaryBitmapPath);

	std::unique_ptr<GameObject> basic_weapon = std::make_unique<GameObject>(basic_weapon_size, PrefabCreationKey());
	basic_weapon->AddComponent(std::make_unique<Firearm>(GetPrefab(EPrefabId::BasicBullet), WPN_BASIC_RELOAD, FirearmType::Basic));
	basic_weapon->AddComponent(std::make_unique<SpriteRenderer>(bitmap));

	InsertPrefab(EPrefabId::BasicFirearm, std::move(basic_weapon));
}

void PrefabFactory::CreateSuperFirearmPrefab()
{
	constexpr Vector super_weapon_size = Vector(30, 10);
	SDL_Surface* bitmap = AssetManager::GetInstance()->GetSurfaceAsset(s_WeaponSuperBitmapPath);

	std::unique_ptr<GameObject> super_weapon = std::make_unique<GameObject>(super_weapon_size, PrefabCreationKey());
	super_weapon->AddComponent(std::make_unique<Firearm>(GetPrefab(EPrefabId::SuperBullet), WPN_SUPER_RELOAD, FirearmType::Super));
	super_weapon->AddComponent(std::make_unique<SpriteRenderer>(bitmap));

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
	GameObject* basic_weapon = GameObject::Instantiate(
		GetPrefab(PrefabFactory::EPrefabId::BasicFirearm)
	);
	basic_weapon->SetPosition(player->GetPosition() + Vector(Direction::EAST) * player->GetSize().x);
	player->AddChild(basic_weapon);

	// Silna broñ
	GameObject* super_weapon = GameObject::Instantiate(
		GetPrefab(PrefabFactory::EPrefabId::SuperFirearm)
	);
	super_weapon->SetPosition(player->GetPosition() + Vector(Direction::EAST) * player->GetSize().x);
	player->AddChild(super_weapon);

	SDL_Surface* player_bitmap = AssetManager::GetInstance()->GetSurfaceAsset(s_PlayerBitmapPath);
	player->AddComponent(std::make_unique<SpriteRenderer>(player_bitmap));

	InsertPrefab(EPrefabId::Player, std::move(player));
}

void PrefabFactory::InsertPrefab(EPrefabId prefab_id, std::unique_ptr<GameObject> prefab)
{
	m_PrefabDict.insert({ prefab_id, std::move(prefab) });
}
