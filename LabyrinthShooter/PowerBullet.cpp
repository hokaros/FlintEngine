#include "PowerBullet.h"
#include "../FlatEngine/ObjectManager.h"

PowerBullet::PowerBullet(float speed, int damage)
	: Bullet(speed, damage)
	, health(POWER_BULLET_HEALTH) 
{

}

void PowerBullet::Awake()
{
	Bullet::Awake();
}

IUpdateable* PowerBullet::Copy()
{
	return new PowerBullet(speed, damage);
}

void PowerBullet::OnCollision(BoxCollider& collider) 
{
	if (collider.GetOwner().isDestroyable) 
	{
		collider.GetOwner().SetDestroyed(true);

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