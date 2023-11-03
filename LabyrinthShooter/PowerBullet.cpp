#include "PowerBullet.h"
#include "../FlatEngine/ObjectManager.h"

PowerBullet::PowerBullet(GameObject& owner, float speed, int damage)
	: Bullet(owner, speed, damage), health(POWER_BULLET_HEALTH) {

}

void PowerBullet::Awake()
{
	Bullet::Awake();
}

ObjectComponent* PowerBullet::Copy(GameObject& newOwner) {
	return new PowerBullet(newOwner, speed, damage);
}

void PowerBullet::OnCollision(BoxCollider& collider) {
	if (collider.GetOwner().isDestroyable) {
		collider.GetOwner().SetDestroyed(true);

		DecreaseHealth();
	}
	else {
		Bullet::OnCollision(collider);
	}
}

void PowerBullet::DecreaseHealth() {
	health--;
	if (health <= 0) {
		GameObject::Destroy(&gameObject);
	}
}