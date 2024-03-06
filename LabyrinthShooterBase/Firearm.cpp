#include "Firearm.h"
#include "../FlatEngine/ObjectManager.h"
#include "../FlatEngine/GameObject.h"

DEFINE_COMPONENT(Firearm);
DEFINE_FIELD(Firearm, m_BulletPrefab);
DEFINE_FIELD(Firearm, m_ReloadTime);
DEFINE_FIELD(Firearm, m_Type);

Firearm::Firearm(PrefabRef bulletPrefab, float reloadTime, FirearmType type)
	: m_BulletPrefab(bulletPrefab)
	, m_ReloadTime(reloadTime)
	, m_Type(type) 
{

}

void Firearm::Update() 
{
	timeSinceLastShot += Timer::Main()->GetDeltaTime();
	if (timeSinceLastShot >= m_ReloadTime) 
	{
		isReloaded = true;
	}
}

bool Firearm::TryShoot() 
{
	if (!isReloaded)
		return false;

	// Stworzenie pocisku
	GameObject* bullet = GameObject::Instantiate(*m_BulletPrefab.Get());

	// Ustawienie pozycji
	Vector relativePos = Vector(Direction::EAST) * m_GameObject->GetSize().x;
	bullet->SetPosition(m_GameObject->LocalToWorld(relativePos));
	// Obrót zgodnie z obrotem broni
	bullet->Rotate(m_GameObject->GetRotation());

	// Nadanie kierunku lotu
	Bullet* b = bullet->FindComponent<Bullet>();
	b->SetDirection(m_GameObject->LookingDirection());

	// Zachowanie po kolizji
	b->onPlayerCollision = onPlayerCollision;

	// Aktualizacja info o prze³adowaniu
	timeSinceLastShot = 0.0f;
	isReloaded = false;

	return true;
}

FirearmType Firearm::GetType() const {
	return m_Type;
}