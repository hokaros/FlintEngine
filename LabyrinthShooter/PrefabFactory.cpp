#include "PrefabFactory.h"
#include "../FlatEngine/RectangleRenderer.h"
#include "PowerBullet.h"

PrefabFactory::PrefabFactory()
{
	CreateBasicBulletPrefab();
	CreateSuperBulletPrefab();
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

void PrefabFactory::InsertPrefab(EPrefabId prefab_id, GameObject* prefab)
{
	m_PrefabDict.insert({ prefab_id, prefab });
}
