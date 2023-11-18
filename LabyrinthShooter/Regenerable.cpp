#include "Regenerable.h"

Regenerable::Regenerable(double regenerationTime)
	: regenerationTime(regenerationTime) 
{
}

void Regenerable::Start()
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

IUpdateable* Regenerable::Copy() 
{
	return new Regenerable(regenerationTime);
}