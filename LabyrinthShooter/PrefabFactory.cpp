#include "PrefabFactory.h"

#include "../FlatEngine/RectangleRenderer.h"
#include "../FlatEngine/AssetManager.h"
#include "../FlatEngine/SpriteRenderer.h"
#include "PowerBullet.h"
#include "Firearm.h"
#include "ConstantMover.h"
#include "PlayerEquipment.h"
#include "PlayerController.h"

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

PrefabFactory::~PrefabFactory()
{
	for (auto& entry : m_PrefabDict)
	{
		GameObject* go = entry.second;
		delete go;
	}
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
	constexpr Vector basic_bullet_size = Vector(4, 4);
	Rgb8 basic_bullet_color = Rgb8(0xFF, 0xFF, 0x00);

	GameObject* basic_bullet = new GameObject(basic_bullet_size, PrefabCreationKey());

	basic_bullet->AddComponent(new Bullet(*basic_bullet, BULLET_BASIC_SPEED, BULLET_BASIC_DAMAGE));
	basic_bullet->AddComponent(new BoxCollider(*basic_bullet, Vector::ZERO, basic_bullet_size));
	basic_bullet->SetRenderer(new RectangleRenderer(*basic_bullet, basic_bullet_color));

	InsertPrefab(EPrefabId::BasicBullet, basic_bullet);
}

void PrefabFactory::CreateSuperBulletPrefab()
{
	constexpr Vector super_bullet_size = Vector(10, 10);
	Rgb8 super_bullet_color = Rgb8(0xFF, 0x00, 0x00);

	GameObject* super_bullet = new GameObject(super_bullet_size, PrefabCreationKey());

	super_bullet->AddComponent(new PowerBullet(*super_bullet, BULLET_SUPER_SPEED, BULLET_SUPER_DAMAGE));
	super_bullet->AddComponent(new BoxCollider(*super_bullet, Vector::ZERO, super_bullet_size));
	super_bullet->SetRenderer(new RectangleRenderer(*super_bullet, super_bullet_color));

	InsertPrefab(EPrefabId::SuperBullet, super_bullet);
}

void PrefabFactory::CreateBasicFirearmPrefab()
{
	constexpr Vector basic_weapon_size = Vector(30, 10);
	SDL_Surface* bitmap = AssetManager::GetInstance()->GetSurfaceAsset(s_WeaponPrimaryBitmapPath);

	GameObject* basic_weapon = new GameObject(basic_weapon_size, PrefabCreationKey());
	basic_weapon->AddComponent(new Firearm(*basic_weapon, GetPrefab(EPrefabId::BasicBullet), WPN_BASIC_RELOAD, FirearmType::Basic));
	basic_weapon->SetRenderer(new SpriteRenderer(*basic_weapon, bitmap));

	InsertPrefab(EPrefabId::BasicFirearm, basic_weapon);
}

void PrefabFactory::CreateSuperFirearmPrefab()
{
	constexpr Vector super_weapon_size = Vector(30, 10);
	SDL_Surface* bitmap = AssetManager::GetInstance()->GetSurfaceAsset(s_WeaponSuperBitmapPath);

	GameObject* super_weapon = new GameObject(super_weapon_size, PrefabCreationKey());
	super_weapon->AddComponent(new Firearm(*super_weapon, GetPrefab(EPrefabId::SuperBullet), WPN_SUPER_RELOAD, FirearmType::Super));
	super_weapon->SetRenderer(new SpriteRenderer(*super_weapon, bitmap));

	InsertPrefab(EPrefabId::SuperFirearm, super_weapon);
}

void PrefabFactory::CreatePlayerPrefab()
{
	constexpr float player_speed = 300.0f;
	constexpr Vector player_size = Vector(20, 20);
	constexpr int player_max_health = 3;

	GameObject* player = new GameObject(player_size, PrefabCreationKey());

	player->AddComponent(new ConstantMover(*player, player_speed));
	player->AddComponent(new BoxCollider(*player, Vector::ZERO, player_size));
	player->AddComponent(new PlayerEquipment(*player));
	player->AddComponent(new Health(*player, player_max_health, nullptr));
	player->AddComponent(new PlayerController(*player));

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
	player->SetRenderer(new SpriteRenderer(*player, player_bitmap));

	InsertPrefab(EPrefabId::Player, player);
}

void PrefabFactory::InsertPrefab(EPrefabId prefab_id, GameObject* prefab)
{
	m_PrefabDict.insert({ prefab_id, prefab });
}
