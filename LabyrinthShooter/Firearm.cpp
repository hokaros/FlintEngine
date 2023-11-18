#include "Firearm.h"
#include "../FlatEngine/ObjectManager.h"

Firearm::Firearm(const GameObject& bulletPrefab, float reloadTime, FirearmType type)
	: bulletPrefab(bulletPrefab)
	, reloadTime(reloadTime)
	, type(type) 
{

}

void Firearm::Update() 
{
	timeSinceLastShot += Timer::Main()->GetDeltaTime();
	if (timeSinceLastShot >= reloadTime) 
	{
		isReloaded = true;
	}
}

bool Firearm::TryShoot() 
{
	if (!isReloaded)
		return false;

	// Stworzenie pocisku
	GameObject* bullet = GameObject::Instantiate(bulletPrefab);

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

IUpdateable* Firearm::Copy() {
	return new Firearm(bulletPrefab, reloadTime, type);
}

FirearmType Firearm::GetType() const {
	return type;
}