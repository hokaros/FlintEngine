#pragma once
#include <ComponentDefinition.h>
#include "StatRenderer.h"
#include <list>
#include <functional>

class Health :
	public ObjectComponent
{
	DECLARE_COMPONENT();

public:
	Health() = default;

	virtual void Awake() override;
	virtual void Start() override;

	void Hurt(int hp);
	bool IsDead() const;

	// Obs³ugiwanie œmierci z argumentem tego komponentu
	void SubscribeDeath(std::function<void(Health*)> handler);

private:
	void OnDeath();
	void NotifyOnDeath();

	StatRenderer* FindStatRenderer() const;

private:
	int m_MaxHealth = 3;
	DECLARE_FIELD(m_MaxHealth);

	bool m_RenderHearts = false;
	DECLARE_FIELD(m_RenderHearts);

	int m_CurrHealth = 0;

	std::list<std::function<void(Health*)>> m_OnDeath;

	StatRenderer* m_HealthRenderer = nullptr;
};

