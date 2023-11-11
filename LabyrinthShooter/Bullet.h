#pragma once
#include "../FlatEngine/ObjectComponent.h"
#include "../FlatEngine/BoxCollider.h"
#include "../FlatEngine/ComponentDefinition.h"
#include "Health.h"

// Obiekt biegn¹cy przed siebie do momentu kolizji
class Bullet :
	public ObjectComponent
{
	DECLARE_COMPONENT();

public:
	function<void(GameObject&, int dmg)> onPlayerCollision;

public:
	Bullet(GameObject& owner);
	Bullet(GameObject& owner, float speed, int damage);
	void Awake() override;
	void Update() override;

	void SetDirection(const Vector& direction);

	virtual ObjectComponent* Copy(GameObject& newOwner) override;

protected:
	float speed = 1.0f;
	int damage = 1;

protected:
	virtual void OnCollision(BoxCollider& collider);

private:
	Vector direction;

	DECLARE_FIELD(speed);
	DECLARE_FIELD(damage);
};

