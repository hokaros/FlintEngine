#pragma once
#include "../FlatEngine/ObjectComponent.h"
#include "StatRenderer.h"
#include <list>
#include <functional>

class Health :
	public ObjectComponent
{
public:
	Health(int maxHealth, StatRenderer* healthRenderer);

	void Hurt(int hp);
	bool IsDead() const;

	void SetStatRenderer(StatRenderer* value);

	// Obs³ugiwanie œmierci z argumentem tego komponentu
	void SubscribeDeath(std::function<void(Health*)> handler);

	virtual std::unique_ptr<ObjectComponent> Copy() override;

private:
	int maxHealth;
	int currHealth;

	std::list<std::function<void(Health*)>> onDeath;

	StatRenderer* healthRenderer;

private:
	void OnDeath();
};

