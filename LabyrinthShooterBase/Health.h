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
	Health(int max_health, StatRenderer* health_renderer);

	virtual void Start() override;

	void Hurt(int hp);
	bool IsDead() const;

	void SetStatRenderer(StatRenderer* value);

	// Obs³ugiwanie œmierci z argumentem tego komponentu
	void SubscribeDeath(std::function<void(Health*)> handler);

private:
	void OnDeath();

private:
	int m_MaxHealth = 3;
	DECLARE_FIELD(m_MaxHealth);

	int m_CurrHealth;

	std::list<std::function<void(Health*)>> m_OnDeath;

	StatRenderer* m_HealthRenderer = nullptr;
};

