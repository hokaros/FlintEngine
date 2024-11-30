#include "Bullet.h"
#include <Core/GameObject.h>

DEFINE_COMPONENT(Bullet);

DEFINE_FIELD(Bullet, speed);
DEFINE_FIELD(Bullet, damage);

Bullet::Bullet(float speed, int damage)
	: speed(speed)
	, m_Direction(Direction::EAST)
	, damage(damage) 
{

}

void Bullet::Awake()
{
	BoxCollider* collider = m_GameObject->FindComponent<BoxCollider>();

	collider->onCollision =
		[this](BoxCollider& c) {
		OnCollision(c);
	};

	m_CurrentLifeTime = 0.0f;
}

void Bullet::Update() 
{
	m_GameObject->Translate(
		m_Direction * speed * Timer::Main()->GetDeltaTime()
	);

	m_CurrentLifeTime += Timer::Main()->GetDeltaTime();
	if (m_CurrentLifeTime >= s_MaxTimeToLive)
	{
		GameObject::Destroy(m_GameObject);
	}
}

void Bullet::OnCollision(BoxCollider& collider) 
{
	GameObject& other_game_object = collider.GetOwner();
	// Obs³uga trafienia gracza
	Health* player_health = other_game_object.FindComponent<Health>();
	if (player_health == m_OwnerHealth)
		return; // Ignoruj

	if (player_health != nullptr) 
	{
		OnCollisionWithHealth(*player_health);
	}

	if (other_game_object.FindComponent<Bullet>() == nullptr)
	{
		GameObject::Destroy(m_GameObject); // zniszczenie pocisku
	}
}

void Bullet::OnCollisionWithHealth(Health& health)
{
	if (onPlayerCollision)
	{
		printf("Bullet collided with a player\n");
		onPlayerCollision(health.GetOwner(), damage);
	}
}

void Bullet::SetDirection(const Vector& direction) 
{
	m_Direction = direction;
	m_Direction.Normalize();
}

void Bullet::SetOwnerHealth(const Health* health)
{
	m_OwnerHealth = health;
}
