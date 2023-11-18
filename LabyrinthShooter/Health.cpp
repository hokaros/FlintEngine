#include "Health.h"

Health::Health(int maxHealth, StatRenderer* healthRenderer)
	: maxHealth(maxHealth)
	, currHealth(maxHealth)
	, healthRenderer(healthRenderer) 
{

	if (healthRenderer != nullptr) 
	{
		healthRenderer->UpdateStat(currHealth);
	}
}

IUpdateable* Health::Copy()
{
	Health* health = new Health(maxHealth, nullptr);
	// Dodanie funkcji obs³ugi œmierci
	for (function<void(Health*)> deathHandler : onDeath) {
		health->SubscribeDeath(deathHandler);
	}
	return health;
}

void Health::Hurt(int hp) 
{
	currHealth -= hp;
	printf("Ouch, curr = %d\n", currHealth);

	if (healthRenderer != nullptr)
		healthRenderer->UpdateStat(currHealth);

	if (currHealth <= 0) 
	{
		OnDeath();
	}
}

bool Health::IsDead() const 
{
	return currHealth <= 0;
}

void Health::SetStatRenderer(StatRenderer* value)
{
	healthRenderer = value;
}

void Health::OnDeath() 
{
	for (function<void(Health*)> handler : onDeath) 
	{
		if (handler) 
		{
			handler(this);
		}
	}
}

void Health::SubscribeDeath(function<void(Health*)> handler) 
{
	onDeath.push_back(handler);
}
