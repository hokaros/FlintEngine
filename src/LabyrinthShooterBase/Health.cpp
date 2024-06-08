#include "Health.h"

#include <Core/GameObject.h>
#include <Core/Scene.h>

DEFINE_COMPONENT(Health);
DEFINE_FIELD(Health, m_MaxHealth);
DEFINE_FIELD(Health, m_RenderHearts);

void Health::Awake()
{
	m_CurrHealth = m_MaxHealth;

	if (m_RenderHearts)
	{
		m_HealthRenderer = FindStatRenderer();
	}
	else
	{
		m_HealthRenderer = nullptr;
	}
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

void Health::OnDeath() 
{
	std::cout << "Dead" << std::endl;

	NotifyOnDeath();

	GameObject::Destroy(&GetOwner());
}

void Health::NotifyOnDeath()
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

StatRenderer* Health::FindStatRenderer() const
{
	// TODO: let's pass this by something other than name
	IGameObject* stats_igo = GetOwner().GetScene()->FindGameObjectByName("Health Stats");
	if (stats_igo == nullptr)
		return nullptr;

	GameObject* stats_go = dynamic_cast<GameObject*>(stats_igo); // TODO: let's think how to remove this
	if (stats_go == nullptr)
		return nullptr;

	return stats_go->FindComponent<BMPStats>();
}
