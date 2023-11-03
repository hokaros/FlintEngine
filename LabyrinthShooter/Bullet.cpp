#include "Bullet.h"
#include "../FlatEngine/ObjectManager.h"
#include "../FlatEngine/utility.h"

Bullet::Bullet(GameObject& owner, float speed, int damage)
	: ObjectComponent(owner), speed(speed), direction(Direction::EAST), damage(damage) {

}

void Bullet::Awake()
{
	BoxCollider* collider = gameObject.FindComponent<BoxCollider>();
	collider->m_IsStatic = true;

	collider->onCollision =
		[this](BoxCollider& c) {
		OnCollision(c);
	};
}

void Bullet::Update() {
	gameObject.Translate(
		direction * speed * Timer::Main()->GetDeltaTime()
	);
}

void Bullet::OnCollision(BoxCollider& collider) {
	GameObject& other_game_object = collider.GetOwner();
	// Obs³uga trafienia gracza
	Health* playerHealth = other_game_object.FindComponent<Health>();
	if (playerHealth != NULL && onPlayerCollision) {
		printf("Bullet collided with a player\n");
		onPlayerCollision(other_game_object, damage);
	}

	GameObject::Destroy(&gameObject); // zniszczenie pocisku
}

ObjectComponent* Bullet::Copy(GameObject& newOwner) {
	return new Bullet(newOwner, speed, damage);
}

void Bullet::SetDirection(const Vector& direction) {
	this->direction = Vector(direction);
	this->direction.Normalize();
}