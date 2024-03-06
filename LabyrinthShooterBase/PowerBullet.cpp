#include "PowerBullet.h"

#include "../FlatEngine/ObjectManager.h"
#include "../FlatEngine/GameObject.h"
#include "Destroyable.h"

DEFINE_COMPONENT_DERIVED(PowerBullet, Bullet);

DEFINE_FIELD(PowerBullet, health);

PowerBullet::PowerBullet(float speed, int damage, int health)
	: Bullet(speed, damage)
	, health(health) 
{

}

void PowerBullet::Awake()
{
	Bullet::Awake();
}

void PowerBullet::OnCollision(BoxCollider& collider) 
{
	Destroyable* destroyable = collider.GetOwner().FindComponent<Destroyable>();
	if (destroyable != nullptr)
	{
		destroyable->SetDestroyed(true);

		DecreaseHealth();
	}
	else 
	{
		Bullet::OnCollision(collider);
	}
}

void PowerBullet::DecreaseHealth() 
{
	health--;
	if (health <= 0) 
	{
		GameObject::Destroy(m_GameObject);
	}
}