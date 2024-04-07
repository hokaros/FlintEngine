#include "PhysicsSystem.h"
#include "BoxCollider.h"
#include "utility.h"
#include "DebugRenderer.h"
#include "DebugConfig.h"
#include "GameObject.h"

PhysicsSystem* PhysicsSystem::s_Instance = nullptr;

PhysicsSystem::PhysicsSystem(SystemCreationKey)
{
	FE_ASSERT(s_Instance == nullptr, "Trying to create another physics system");
	s_Instance = this;
}

PhysicsSystem::~PhysicsSystem()
{
	if (s_Instance == this)
	{
		s_Instance = nullptr;
	}
}

void PhysicsSystem::Update()
{
	ApplyUnregisterColliders();

	// TODO: change to QuadTree
	for (int i = 0; i < m_Colliders.size(); i++)
	{
		BoxCollider* collider1 = m_Colliders[i];
		if (!collider1->GetOwner().IsEnabled())
			continue;

		for (int j = 0; j < i; j++)
		{
			BoxCollider* collider2 = m_Colliders[j];

			if (!collider2->GetOwner().IsEnabled())
				continue;

			if (collider1->DoesCollide(*collider2))
			{
				OnCollision(*collider1, *collider2);
			}
		}
	}
}

void PhysicsSystem::DebugRender()
{
	if (debug::DebugConfig::ShouldDebugRenderPhysics())
	{
		DebugDrawColliders();
	}
}

PhysicsSystem* PhysicsSystem::GetInstance()
{
	return s_Instance;
}

void PhysicsSystem::RegisterCollider(BoxCollider* collider)
{
	FE_ASSERT(collider != nullptr, "collider should not be null");
	m_Colliders.push_back(collider);
}

void PhysicsSystem::UnregisterCollider(BoxCollider* collider)
{
	m_CollidersToUnregister.push_back(collider);
}

void PhysicsSystem::OnCollision(BoxCollider& col1, BoxCollider& col2)
{
	col1.OnCollision(col2);
	col2.OnCollision(col1);

	Discollide(col1, col2);
}

void PhysicsSystem::Discollide(BoxCollider& col1, BoxCollider& col2)
{
	if (col1.IsStatic() && col2.IsStatic())
		return;

	Rect intersection = col1.GetIntersection(col2);
	Vector intersection_mid = intersection.GetMiddle();

	auto clamp_to_half_size = [](Vector& v, const Vector& size)
	{
		float half_size_x = abs(size.x) / 2.0f;
		if (abs(v.x) > half_size_x)
		{
			v.x = half_size_x * v.x / abs(v.x);
		}

		float half_size_y = abs(size.y) / 2.0f;
		if (abs(v.y) > half_size_y)
		{
			v.y = half_size_y * v.y / abs(v.y);
		}
	};

	Vector to_col1 = col1.GetWorldMiddle() - intersection_mid;
	clamp_to_half_size(to_col1, intersection.size);

	Vector to_col2 = col2.GetWorldMiddle() - intersection_mid;
	clamp_to_half_size(to_col2, intersection.size);

	if (!col1.IsStatic())
	{
		col1.GetOwner().Translate(to_col1);
	}
	if (!col2.IsStatic())
	{
		col2.GetOwner().Translate(to_col2);
	}
}

void PhysicsSystem::BumpOut(BoxCollider& bump_target, BoxCollider& bump_source) {
	// Tylko jeœli koliduje z other
	Rect intersection = bump_source.GetIntersection(bump_target);

	Vector outVector = bump_target.GetWorldMiddle() - intersection.GetMiddle();
	if (outVector.Length() == 0) {
		outVector = Vector(0, -FLT_EPSILON);
	}

	float speed = BUMPOUT_SPEED / intersection.CalculateArea();
	outVector.Normalize();
	Vector dPos = outVector * speed * Timer::Main()->GetDeltaTime();
	std::cout << "Bumping for: " << dPos << std::endl;
	bump_target.GetOwner().Translate(dPos);
}

void PhysicsSystem::DebugDrawColliders() const
{
	for (BoxCollider* collider : m_Colliders)
	{
		Rgb8 color = Rgb8(0x00, 0x00, 0xFF);
		if (collider->IsStatic())
		{
			color = Rgb8(0xFF, 0x00, 0x00);
		}
		if (!collider->GetOwner().IsEnabled())
		{
			color = Rgb8(0xFF, 0xFF, 0xFF);
		}

		Vector col_min = collider->GetWorldPos();
		Vector col_size = collider->GetWorldSize();
		Rect rect = Rect(col_min, col_size);
		DebugRenderer::DrawWireRect(rect, color);
	}
}

void PhysicsSystem::ApplyUnregisterColliders()
{
	for (BoxCollider* collider : m_CollidersToUnregister)
	{
		for (auto it = m_Colliders.begin(); it != m_Colliders.end(); it++)
		{
			if (*it == collider)
			{
				m_Colliders.erase(it);
				break;
			}
		}
	}

	m_CollidersToUnregister.clear();
}
