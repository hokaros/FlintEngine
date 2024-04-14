#include "Regenerable.h"
#include <Core/GameObject.h>

DEFINE_COMPONENT(Regenerable);

Regenerable::Regenerable(double regenerationTime)
	: m_RegenerationTime(regenerationTime) 
{
}

void Regenerable::Awake()
{
	destroyable = m_GameObject->FindComponent<Destroyable>();
	if (destroyable == nullptr)
	{
		FE_DATA_ERROR("Regenerable without destroyable: %s", m_GameObject->GetName().c_str());
		return;
	}

	destroyable->SubscribeDestroyed([this](Destroyable&) {OnDestroyed(); });
}

void Regenerable::Update() 
{

}

void Regenerable::OnDestroyed() 
{
	if (!destroyable->IsDestroyed())
		return; // tylko zniszczenie

	Timer::Main()->InvokeOnNextFrame([this]() 
		{
		destroyable->SetDestroyed(false);
		},
	m_RegenerationTime
	);
}