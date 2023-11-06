#include "PrefabFactory.h"
#include "../FlatEngine/RectangleRenderer.h"
#include "PowerBullet.h"
#include "Firearm.h"

PrefabFactory::PrefabFactory()
{
	CreateBasicBulletPrefab();
	CreateSuperBulletPrefab();
	CreateBasicFirearmPrefab();
	CreateSuperFirearmPrefab();
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

	GameObject* basic_weapon = new GameObject(basic_weapon_size, PrefabCreationKey());
	basic_weapon->AddComponent(new Firearm(*basic_weapon, GetPrefab(EPrefabId::BasicBullet), WPN_BASIC_RELOAD, FirearmType::Basic));

	InsertPrefab(EPrefabId::BasicFirearm, basic_weapon);
}

void PrefabFactory::CreateSuperFirearmPrefab()
{
	constexpr Vector super_weapon_size = Vector(30, 10);

	GameObject* super_weapon = new GameObject(super_weapon_size, PrefabCreationKey());
	super_weapon->AddComponent(new Firearm(*super_weapon, GetPrefab(EPrefabId::SuperBullet), WPN_SUPER_RELOAD, FirearmType::Super));

	InsertPrefab(EPrefabId::SuperFirearm, super_weapon);
}

void PrefabFactory::InsertPrefab(EPrefabId prefab_id, GameObject* prefab)
{
	m_PrefabDict.insert({ prefab_id, prefab });
}
