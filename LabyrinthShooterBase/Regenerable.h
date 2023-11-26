#pragma once
#include "../FlatEngine/ComponentDefinition.h"

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
	double regenerationTime = 1.0f;

private:
	void OnDestroyed();
};

