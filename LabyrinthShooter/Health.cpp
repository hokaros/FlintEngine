#include "Health.h"

Health::Health(GameObject& owner, int maxHealth, StatRenderer* healthRenderer)
	: ObjectComponent(owner), maxHealth(maxHealth), currHealth(maxHealth), healthRenderer(healthRenderer) {

	if (healthRenderer != NULL) {
		healthRenderer->UpdateStat(currHealth);
	}
}

ObjectComponent* Health::Copy(GameObject& newOwner)
{
	Health* health = new Health(newOwner, maxHealth, NULL);
	// Dodanie funkcji obs³ugi œmierci
	for (function<void(Health*)> deathHandler : onDeath) {
		health->SubscribeDeath(deathHandler);
	}
	return health;
}

void Health::Hurt(int hp) {
	currHealth -= hp;
	printf("Ouch, curr = %d\n", currHealth);

	if (healthRenderer != NULL)
		healthRenderer->UpdateStat(currHealth);

	if (currHealth <= 0) {
		OnDeath();
	}
}

bool Health::IsDead() const {
	return currHealth <= 0;
}

void Health::OnDeath() {
	for (function<void(Health*)> handler : onDeath) {
		if (handler) {
			handler(this);
		}
	}
}

void Health::SubscribeDeath(function<void(Health*)> handler) {
	onDeath.push_back(handler);
}
