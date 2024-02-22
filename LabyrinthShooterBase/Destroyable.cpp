#include "Destroyable.h"
#include "../FlatEngine/GameObject.h"

DEFINE_COMPONENT(Destroyable);

void Destroyable::SubscribeDestroyed(std::function<void(Destroyable&)> handler)
{
	onDestroyedChanged.push_back(handler);
}

bool Destroyable::IsDestroyed() const
{
	return !m_GameObject->IsEnabled();
}

void Destroyable::SetDestroyed(bool destroyed)
{
	m_GameObject->SetEnabled(!destroyed);

	for (function<void(Destroyable&)> handler : onDestroyedChanged)
	{
		if (handler)
			handler(*this);
	}
}

std::unique_ptr<ObjectComponent> Destroyable::Copy()
{
	return std::make_unique<Destroyable>();
}
