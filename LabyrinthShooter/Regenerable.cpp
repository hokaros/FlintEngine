#include "Regenerable.h"
#include "../FlatEngine/GameObject.h"

Regenerable::Regenerable(double regenerationTime)
	: regenerationTime(regenerationTime) 
{
}

void Regenerable::Awake()
{
	m_GameObject->SubscribeDestroyed([this](GameObject*) {OnDestroyed(); });
}

void Regenerable::Update() 
{

}

void Regenerable::OnDestroyed() 
{
	if (!m_GameObject->IsDestroyed())
		return; // tylko zniszczenie

	Timer::Main()->InvokeOnNextFrame([this]() 
		{
		m_GameObject->SetDestroyed(false);
		},
	regenerationTime
	);
}

std::unique_ptr<ObjectComponent> Regenerable::Copy()
{
	return std::make_unique<Regenerable>(regenerationTime);
}