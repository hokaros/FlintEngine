#pragma once
#include "../FlatEngine/ObjectComponent.h"
#include "StatRenderer.h"
#include <list>

using std::function;

class Health :
	public ObjectComponent
{
public:
	Health(int maxHealth, StatRenderer* healthRenderer);

	void Hurt(int hp);
	bool IsDead() const;

	void SetStatRenderer(StatRenderer* value);

	// Obs³ugiwanie œmierci z argumentem tego komponentu
	void SubscribeDeath(function<void(Health*)> handler);

	virtual IUpdateable* Copy() override;

private:
	int maxHealth;
	int currHealth;

	std::list<function<void(Health*)>> onDeath;

	StatRenderer* healthRenderer;

private:
	void OnDeath();
};

