#pragma once
#include "../FlatEngine/ObjectComponent.h"

class Regenerable :
	public ObjectComponent
{
public:
	Regenerable(double regenerationTime);

	virtual void Start() override;
	virtual void Update() override;

	virtual IUpdateable* Copy() override;

private:
	double regenerationTime;

private:
	void OnDestroyed();
};

