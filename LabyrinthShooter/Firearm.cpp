#include "Firearm.h"

Firearm::Firearm(GameObject& owner, const GameObject& bulletPrefab, float reloadTime, FirearmType type)
	: ObjectComponent(owner), bulletPrefab(bulletPrefab), reloadTime(reloadTime), type(type) {

	gameObject.collisionEnabled = false;
}

void Firearm::Update() {
	timeSinceLastShot += Timer::Main()->GetDeltaTime();
	if (timeSinceLastShot >= reloadTime) {
		isReloaded = true;
	}
}

bool Firearm::TryShoot() {
	if (!isReloaded)
		return false;

	// Stworzenie pocisku
	GameObject* bullet = new GameObject(bulletPrefab);

	ObjectManager::Main()->AddObject(bullet);

	// Ustawienie pozycji
	Vector relativePos = Vector(Direction::EAST) * gameObject.GetSize().x;
	bullet->SetPosition(gameObject.LocalToWorld(relativePos));
	// Obrót zgodnie z obrotem broni
	bullet->Rotate(gameObject.GetRotation());

	// Nadanie kierunku lotu
	Bullet* b = bullet->FindComponent<Bullet>();
	b->SetDirection(gameObject.LookingDirection());

	// Zachowanie po kolizji
	b->onPlayerCollision = onPlayerCollision;
	bullet->bumping = false;

	// Aktualizacja info o prze³adowaniu
	timeSinceLastShot = 0.0f;
	isReloaded = false;

	return true;
}

ObjectComponent* Firearm::Copy(GameObject& newOwner) {
	return new Firearm(newOwner, bulletPrefab, reloadTime, type);
}

FirearmType Firearm::GetType() const {
	return type;
}