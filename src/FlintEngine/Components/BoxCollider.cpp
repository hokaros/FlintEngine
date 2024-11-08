#include "BoxCollider.h"
#include "PhysicsSystem.h"
#include <Core/GameObject.h>

DEFINE_COMPONENT(BoxCollider);

DEFINE_FIELD(BoxCollider, m_InsideOutCollision);
DEFINE_FIELD(BoxCollider, m_Position);
DEFINE_FIELD(BoxCollider, m_Size);
DEFINE_FIELD(BoxCollider, m_IsStatic);

bool BoxCollider::DoesCollide(const BoxCollider& other) const
{
	// Rozpatrzenie kolizji odwrotnych (mog¹ siê znosiæ)
	bool inverse = false;
	if (m_InsideOutCollision)
		inverse = !inverse;
	if (other.m_InsideOutCollision)
		inverse = !inverse;

	bool intersect = DoesIntersect(other);
	if (inverse) {
		if (IsInside(*this, other))
			return false;  // wewn¹trz
		if (intersect)
			return true;  // na zewn¹trz
	}

	return intersect;
}

void BoxCollider::OnCollision(BoxCollider& other)
{
	if (onCollision)
	{
		onCollision(other);
	}
}

void BoxCollider::SetSize(const Vector& size)
{
	m_Size = size;
}

Vector BoxCollider::GetWorldPos() const
{
	GameObject& go = GetOwner();
	return go.TransformPoint(m_Position);
}

Vector BoxCollider::GetWorldMiddle() const
{
	return GetWorldPos();
}

Vector BoxCollider::GetWorldSize() const
{
	return m_Size.GetScaled(GetOwner().GetWorldScale());
}

void BoxCollider::Awake()
{
	if (PhysicsSystem* physics_system = PhysicsSystem::GetInstance())
	{
		PhysicsSystem::GetInstance()->RegisterCollider(this);
	}
}

void BoxCollider::OnDestroy()
{
	if (PhysicsSystem* physics_system = PhysicsSystem::GetInstance())
	{
		physics_system->UnregisterCollider(this);
	}
}

bool BoxCollider::DoesIntersect(const BoxCollider& other) const
{
	Vector my_pos = GetWorldPos() - GetWorldSize() / 2.f;
	Vector other_pos = other.GetWorldPos() - other.GetWorldSize() / 2.f;
	Vector my_size = GetWorldSize();
	Vector other_size = other.GetWorldSize();

	float yMin1 = my_pos.y;
	float yMax1 = yMin1 + my_size.y; // TODO: world-space size
	float yMin2 = other_pos.y;
	float yMax2 = yMin2 + other_size.y;

	if (!DoLinesIntersect(yMin1, yMax1, yMin2, yMax2))
		return false;

	float xMin1 = my_pos.x;
	float xMax1 = xMin1 + my_size.x;
	float xMin2 = other_pos.x;
	float xMax2 = xMin2 + other_size.x;

	if (!DoLinesIntersect(xMin1, xMax1, xMin2, xMax2))
		return false;

	return true;
}

bool BoxCollider::DoLinesIntersect(float min1, float max1, float min2, float max2)
{
	return (min1 >= min2 && min1 <= max2)
		|| (max1 >= min2 && max1 <= max2)
		|| (min2 >= min1 && min2 <= max1)
		|| (max2 >= min1 && max2 <= max1);
}

bool BoxCollider::IsInside(const BoxCollider& collider1, const BoxCollider& collider2)
{
	Vector pos1 = collider1.GetWorldPos() - collider1.GetWorldSize()/2.f;
	Vector pos2 = collider2.GetWorldPos() - collider2.GetWorldSize()/2.f;
	Vector size1 = collider1.GetWorldSize();
	Vector size2 = collider2.GetWorldSize();

	float yMin1 = pos1.y;
	float yMax1 = yMin1 + size1.y;
	float yMin2 = pos2.y;
	float yMax2 = yMin2 + size2.y;

	if (!IsLineInside(yMin1, yMax1, yMin2, yMax2))
		return false;


	float xMin1 = pos1.x;
	float xMax1 = xMin1 + size1.x;
	float xMin2 = pos2.x;
	float xMax2 = xMin2 + size2.x;

	if (!IsLineInside(xMin1, xMax1, xMin2, xMax2))
		return false;

	return true;
}

bool BoxCollider::IsLineInside(float min1, float max1, float min2, float max2) {
	return (min1 >= min2 && max1 <= max2)
		|| (min2 >= min1 && max2 <= max1);
}

Vector BoxCollider::LinesIntersection(float min1, float max1, float min2, float max2) {
	if (!DoLinesIntersect(min1, max1, min2, max2))
		return Vector(0, 0);

	float start = (min1 < min2) ? min2 : min1;
	float end = (max1 < max2) ? max1 : max2;
	return Vector(start, end);
}

Rect BoxCollider::GetIntersection(const BoxCollider& other) const {
	Vector my_pos = GetWorldPos() - GetWorldSize() / 2.f;
	Vector other_pos = other.GetWorldPos() - other.GetWorldSize() / 2.f;
	Vector my_size = GetWorldSize();
	Vector other_size = other.GetWorldSize();

	float yMin1 = my_pos.y;
	float yMax1 = yMin1 + my_size.y;
	float yMin2 = other_pos.y;
	float yMax2 = yMin2 + other_size.y;

	Vector yIntersection = LinesIntersection(yMin1, yMax1, yMin2, yMax2);

	float xMin1 = my_pos.x;
	float xMax1 = xMin1 + my_size.x;
	float xMin2 = other_pos.x;
	float xMax2 = xMin2 + other_size.x;

	Vector xIntersection = LinesIntersection(xMin1, xMax1, xMin2, xMax2);

	Vector size(
		xIntersection.y - xIntersection.x,
		yIntersection.y - yIntersection.x
	);
	return Rect(
		Vector(xIntersection.x, yIntersection.x),
		size
	);
}

bool BoxCollider::IsStatic() const
{
	return m_IsStatic;
}

void BoxCollider::GetVertices(std::vector<Vector>& out_vertices) const
{
	const Vector top_left = m_Position - Vector(m_Size.x, m_Size.y) / 2.f;
	const Vector top_right = top_left + Vector(m_Size.x, 0);
	const Vector bottom_left = top_left + Vector(0, m_Size.y);
	const Vector bottom_right = bottom_left + Vector(m_Size.x, 0);

	const GameObject& owner = GetOwner();
	out_vertices.push_back(owner.TransformPoint(top_left));
	out_vertices.push_back(owner.TransformPoint(top_right));
	out_vertices.push_back(owner.TransformPoint(bottom_left));
	out_vertices.push_back(owner.TransformPoint(bottom_right));
}
