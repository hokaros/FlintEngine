#pragma once
#include "../FlatEngine/ComponentDefinition.h"
#include "StatRenderer.h"
#include <list>
#include <functional>

class Health :
	public ObjectComponent
{
	DECLARE_COMPONENT();

public:
	Health();
	Health(int maxHealth, StatRenderer* healthRenderer);

	void Hurt(int hp);
	bool IsDead() const;

	void SetStatRenderer(StatRenderer* value);

	// Obs³ugiwanie œmierci z argumentem tego komponentu
	void SubscribeDeath(std::function<void(Health*)> handler);

	virtual std::unique_ptr<ObjectComponent> Copy() override;

private:
	int maxHealth = 3;
	int currHealth;

	std::list<std::function<void(Health*)>> onDeath;

	StatRenderer* healthRenderer = nullptr;

private:
	void OnDeath();
};

