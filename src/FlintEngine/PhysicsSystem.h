#pragma once
#include <vector>
#include "SystemCreationKey.h"

#define BUMPOUT_SPEED 10000.0

class BoxCollider;

class PhysicsSystem
{
public:
	PhysicsSystem(SystemCreationKey);
	~PhysicsSystem();

	void RegisterCollider(BoxCollider* collider); // TODO: change to ICollider
	void UnregisterCollider(BoxCollider* collider);

	void Update();
	void DebugRender();

	static PhysicsSystem* GetInstance();

private:
	void OnCollision(BoxCollider& col1, BoxCollider& col2);

	// Odsu� koliduj�ce obiekty
	void Discollide(BoxCollider& col1, BoxCollider& col2);
	void BumpOut(BoxCollider& bump_target, BoxCollider& bump_source);

	void DebugDrawColliders() const; // TODO: move out

	void ApplyUnregisterColliders();

private:
	std::vector<BoxCollider*> m_Colliders;
	std::vector<BoxCollider*> m_CollidersToUnregister;

	static PhysicsSystem* s_Instance;
};

