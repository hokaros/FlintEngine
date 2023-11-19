#pragma once
#include "../FlatEngine/ObjectComponent.h"

class Regenerable :
	public ObjectComponent
{
public:
	Regenerable(double regenerationTime);

	virtual void Awake() override;
	virtual void Update() override;

	virtual std::unique_ptr<ObjectComponent> Copy() override;

private:
	double regenerationTime;

private:
	void OnDestroyed();
};

