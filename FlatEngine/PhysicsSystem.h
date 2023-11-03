#pragma once
#include <vector>
#include "SystemCreationKey.h"


class BoxCollider;

class PhysicsSystem
{
public:
	PhysicsSystem(SystemCreationKey);

	void RegisterCollider(BoxCollider* collider); // TODO: change to ICollider
	void UnregisterCollider(BoxCollider* collider);

	void Update();
	void DebugRender();

	static PhysicsSystem* GetInstance();

private:
	void OnCollision(BoxCollider& col1, BoxCollider& col2);

	// Odsuñ koliduj¹ce obiekty
	void Discollide(BoxCollider& col1, BoxCollider& col2);
	void BumpOut(BoxCollider& bump_target, BoxCollider& bump_source);

	void DebugDrawColliders() const; // TODO: move out

private:
	std::vector<BoxCollider*> m_Colliders;

	static PhysicsSystem* s_Instance;
};

