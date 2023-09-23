#pragma once
#include "ObjectComponent.h"
class Regenerable :
	public ObjectComponent
{
public:
	Regenerable(GameObject& owner, double regenerationTime);

	void Update() override;

	ObjectComponent* Copy(GameObject& newOwner) override;
private:
	double regenerationTime;
private:
	void OnDestroyed();
};

