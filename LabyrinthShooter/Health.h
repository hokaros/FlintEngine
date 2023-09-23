#pragma once
#include "ObjectComponent.h"
#include "StatRenderer.h"
#include <list>

using std::function;

class Health :
	public ObjectComponent
{
public:
	Health(GameObject& owner, int maxHealth, StatRenderer* healthRenderer);

	void Hurt(int hp);
	bool IsDead() const;

	// Obs³ugiwanie œmierci z argumentem tego komponentu
	void SubscribeDeath(function<void(Health*)> handler);

	virtual ObjectComponent* Copy(GameObject& newOwner) override;

private:
	int maxHealth;
	int currHealth;

	std::list<function<void(Health*)>> onDeath;

	StatRenderer* healthRenderer;

private:
	void OnDeath();
};

