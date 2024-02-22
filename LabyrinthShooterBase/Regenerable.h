#pragma once
#include "../FlatEngine/ComponentDefinition.h"
#include "Destroyable.h"

class Regenerable :
	public ObjectComponent
{
	DECLARE_COMPONENT();
public:
	Regenerable() = default;
	Regenerable(double regenerationTime);

	virtual void Awake() override;
	virtual void Update() override;

	virtual std::unique_ptr<ObjectComponent> Copy() override;

private:
	void OnDestroyed();

private:
	double m_RegenerationTime = 1.0f;

	Destroyable* destroyable = nullptr;
};

