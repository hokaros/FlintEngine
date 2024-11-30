#include "Firearm.h"
#include <ObjectManager.h>
#include <Core/GameObject.h>

DEFINE_COMPONENT(Firearm);
DEFINE_FIELD(Firearm, m_BulletPrefab);
DEFINE_FIELD(Firearm, m_ReloadTime);
DEFINE_FIELD(Firearm, m_Type);
DEFINE_FIELD(Firearm, m_BulletOffset);

void Firearm::Awake()
{
	GameObject* player = GetOwner().GetParent(); // TODO: this reference should be provided from data
	FE_DATA_CHECK(player != nullptr, "Parent of a weapon should be a player");
	m_OwnerHealth = player->FindComponent<Health>();
}

void Firearm::Update()
{
	m_TimeSinceLastShot += Timer::Main()->GetDeltaTime();
	if (m_TimeSinceLastShot >= m_ReloadTime) 
	{
		m_IsReloaded = true;
	}
}

bool Firearm::TryShoot() 
{
	if (!m_IsReloaded)
		return false;

	// Stworzenie pocisku
	GameObject* bullet = GameObject::Instantiate(*m_BulletPrefab.Get());

	// Ustawienie pozycji
	Vector relativePos = Vector(Direction::EAST) * m_BulletOffset;
	bullet->SetWorldPosition(m_GameObject->TransformPoint(relativePos));
	// Obrót zgodnie z obrotem broni
	bullet->Rotate(m_GameObject->GetWorldRotation());

	// Nadanie kierunku lotu
	Bullet* b = bullet->FindComponent<Bullet>();
	b->SetDirection(m_GameObject->LookingDirection());

	// Zachowanie po kolizji
	b->onPlayerCollision = OnPlayerCollision;
	b->SetOwnerHealth(m_OwnerHealth);

	// Aktualizacja info o prze³adowaniu
	m_TimeSinceLastShot = 0.0f;
	m_IsReloaded = false;

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