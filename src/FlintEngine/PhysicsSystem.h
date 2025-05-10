#pragma once
#include <vector>
#include <optional>
#include <Math/Vector.h>
#include "SystemCreationKey.h"

#define BUMPOUT_SPEED 10000.0

class BoxCollider;

struct RaycastHit
{
	Vector collision_point;
};

class PhysicsSystem
{
public:
	PhysicsSystem(SystemCreationKey);
	~PhysicsSystem();

	void RegisterCollider(BoxCollider* collider); // TODO: change to ICollider
	void UnregisterCollider(BoxCollider* collider);

	void Update();
	void DebugRender();

	/**
	* Checks for collision points with the specified ray.
	* If more than one collider collides, returns a random collision.
	*/
	std::optional<RaycastHit> RaycastAny(Vector from, Vector to);

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

