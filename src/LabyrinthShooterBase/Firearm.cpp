#include "Firearm.h"
#include <ObjectManager.h>
#include <Core/GameObject.h>

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
	Vector relativePos = Vector(Direction::EAST) * m_GameObject->GetWorldScale().x; // TODO: local scale
	bullet->SetWorldPosition(m_GameObject->LocalToWorld(relativePos));
	// Obrót zgodnie z obrotem broni
	bullet->Rotate(m_GameObject->GetWorldRotation());

	// Nadanie kierunku lotu
	Bullet* b = bullet->FindComponent<Bullet>();
	b->SetDirection(m_GameObject->LookingDirection());

	// Zachowanie po kolizji
	b->onPlayerCollision = OnPlayerCollision;

	// Aktualizacja info o prze³adowaniu
	timeSinceLastShot = 0.0f;
	isReloaded = false;

	return true;
}

FirearmType Firearm::GetType() const 
{
	return m_Type;
}

void Firearm::OnPlayerCollision(GameObject& player, int dmg)
{
	player.FindComponent<Health>()->Hurt(dmg);
}