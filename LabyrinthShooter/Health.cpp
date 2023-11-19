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

std::unique_ptr<ObjectComponent> Health::Copy()
{
	std::unique_ptr<Health> health = std::make_unique<Health>(maxHealth, nullptr);
	// Dodanie funkcji obs³ugi œmierci
	for (std::function<void(Health*)> deathHandler : onDeath) {
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
	for (std::function<void(Health*)> handler : onDeath) 
	{
		if (handler) 
		{
			handler(this);
		}
	}
}

void Health::SubscribeDeath(std::function<void(Health*)> handler) 
{
	onDeath.push_back(handler);
}
