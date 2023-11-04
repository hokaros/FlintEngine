#include "BoxCollider.h"
#include "PhysicsSystem.h"

BoxCollider::BoxCollider(GameObject& owner, Vector local_pos, Vector size, bool inside_out)
	: ObjectComponent(owner)
	, m_Position(local_pos)
	, m_Size(size)
	, m_InsideOutCollision(inside_out)
{
}

bool BoxCollider::DoesCollide(const BoxCollider& other) const
{
	// Rozpatrzenie kolizji odwrotnych (mog� si� znosi�)
	bool inverse = false;
	if (m_InsideOutCollision)
		inverse = !inverse;
	if (other.m_InsideOutCollision)
		inverse = !inverse;

	bool intersect = DoesIntersect(other);
	if (inverse) {
		if (IsInside(*this, other))
			return false;  // wewn�trz
		if (intersect)
			return true;  // na zewn�trz
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
	return m_Position + go.GetPosition();
}

Vector BoxCollider::GetWorldMiddle() const
{
	return GetWorldPos() + GetWorldSize() / 2.0f;
}

Vector BoxCollider::GetWorldSize() const
{
	return m_Size;
}

void BoxCollider::Awake()
{
	PhysicsSystem::GetInstance()->RegisterCollider(this);
}

void BoxCollider::OnDestroy()
{
	PhysicsSystem::GetInstance()->UnregisterCollider(this);
}

ObjectComponent* BoxCollider::Copy(GameObject& new_owner)
{
	return new BoxCollider(new_owner, m_Position, m_Size);
}

bool BoxCollider::DoesIntersect(const BoxCollider& other) const
{
	Vector my_pos = GetWorldPos();
	Vector other_pos = other.GetWorldPos();
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
	Vector pos1 = collider1.GetWorldPos();
	Vector pos2 = collider2.GetWorldPos();
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
	Vector my_pos = GetWorldPos();
	Vector other_pos = other.GetWorldPos();
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