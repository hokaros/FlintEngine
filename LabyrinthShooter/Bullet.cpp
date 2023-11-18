#include "Bullet.h"

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
	collider->m_IsStatic = true;

	collider->onCollision =
		[this](BoxCollider& c) {
		OnCollision(c);
	};
}

void Bullet::Update() 
{
	m_GameObject->Translate(
		direction * speed * Timer::Main()->GetDeltaTime()
	);
}

void Bullet::OnCollision(BoxCollider& collider) 
{
	GameObject& other_game_object = collider.GetOwner();
	// Obs³uga trafienia gracza
	Health* playerHealth = other_game_object.FindComponent<Health>();
	if (playerHealth != NULL && onPlayerCollision) 
	{
		printf("Bullet collided with a player\n");
		onPlayerCollision(other_game_object, damage);
	}

	GameObject::Destroy(m_GameObject); // zniszczenie pocisku
}

IUpdateable* Bullet::Copy() 
{
	return new Bullet(speed, damage);
}

void Bullet::SetDirection(const Vector& direction) 
{
	this->direction = Vector(direction);
	this->direction.Normalize();
}