#include "Bullet.h"
#include <Core/GameObject.h>

DEFINE_COMPONENT(Bullet);

DEFINE_FIELD(Bullet, speed);
DEFINE_FIELD(Bullet, damage);

Bullet::Bullet(float speed, int damage)
	: speed(speed)
	, direction(Direction::EAST)
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

	currentLifeTime = 0.0f;
}

void Bullet::Update() 
{
	m_GameObject->Translate(
		direction * speed * Timer::Main()->GetDeltaTime()
	);

	currentLifeTime += Timer::Main()->GetDeltaTime();
	if (currentLifeTime >= s_MaxTimeToLive)
	{
		GameObject::Destroy(m_GameObject);
	}
}

void Bullet::OnCollision(BoxCollider& collider) 
{
	GameObject& other_game_object = collider.GetOwner();
	// Obs�uga trafienia gracza
	Health* playerHealth = other_game_object.FindComponent<Health>();
	if (playerHealth != NULL && onPlayerCollision) 
	{
		printf("Bullet collided with a player\n");
		onPlayerCollision(other_game_object, damage);
	}

	if (other_game_object.FindComponent<Bullet>() == nullptr)
	{
		GameObject::Destroy(m_GameObject); // zniszczenie pocisku
	}
}

void Bullet::SetDirection(const Vector& direction) 
{
	this->direction = Vector(direction);
	this->direction.Normalize();
}