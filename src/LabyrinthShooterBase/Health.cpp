#include "Health.h"

DEFINE_COMPONENT(Health);
DEFINE_FIELD(Health, m_MaxHealth);

Health::Health()
{
	m_CurrHealth = m_MaxHealth;
}

Health::Health(int max_health, StatRenderer* health_renderer)
	: m_MaxHealth(max_health)
	, m_CurrHealth(max_health)
	, m_HealthRenderer(health_renderer) 
{

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
	for (std::function<void(Health*)> handler : m_OnDeath)
	{
		if (handler) 
		{
			handler(this);
		}
	}
}

void Health::SubscribeDeath(std::function<void(Health*)> handler) 
{
	m_OnDeath.push_back(handler);
}
