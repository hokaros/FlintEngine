#pragma once
#include <ComponentDefinition.h>

class Destroyable
	: public ObjectComponent
{
	DECLARE_COMPONENT()
public:
	Destroyable() = default;

	void SubscribeDestroyed(std::function<void(Destroyable&)> handler);

	bool IsDestroyed() const;
	void SetDestroyed(bool destroyed);

private:
	std::list<std::function<void(Destroyable&)>> onDestroyedChanged;
};

