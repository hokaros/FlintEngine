#include "Health.h"

#include <Core/GameObject.h>

DEFINE_COMPONENT(Health);
DEFINE_FIELD(Health, m_MaxHealth);

void Health::Awake()
{
	m_CurrHealth = m_MaxHealth;
}

void Health::Start()
{
	if (m_HealthRenderer != nullptr)
	{
		m_HealthRenderer->UpdateStat(m_CurrHealth);
	}
}

void Health::Hurt(int hp) 
{
	m_CurrHealth -= hp;
	printf("Ouch, curr = %d\n", m_CurrHealth);

	if (m_HealthRenderer != nullptr)
		m_HealthRenderer->UpdateStat(m_CurrHealth);

	if (m_CurrHealth <= 0)
	{
		OnDeath();
	}
}

bool Health::IsDead() const 
{
	return m_CurrHealth <= 0;
}

void Health::SetStatRenderer(StatRenderer* value)
{
	m_HealthRenderer = value;
}

void Health::OnDeath() 
{
	std::cout << "Dead" << std::endl;

	for (std::function<void(Health*)> handler : m_OnDeath)
	{
		if (handler) 
		{
			handler(this);
		}
	}

	GameObject::Destroy(&GetOwner());
}

void Health::SubscribeDeath(std::function<void(Health*)> handler) 
{
	m_OnDeath.push_back(handler);
}
