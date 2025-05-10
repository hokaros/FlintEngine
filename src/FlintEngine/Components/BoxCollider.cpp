#include "BoxCollider.h"

#include "PhysicsSystem.h"
#include <Core/GameObject.h>

DEFINE_COMPONENT(BoxCollider);

DEFINE_FIELD(BoxCollider, m_InsideOutCollision);
DEFINE_FIELD(BoxCollider, m_Position);
DEFINE_FIELD(BoxCollider, m_Size);
DEFINE_FIELD(BoxCollider, m_IsStatic);
DEFINE_FIELD(BoxCollider, m_ShouldNavmeshIgnore);

BoxCollider::BoxCollider(Vector local_position, Vector local_size)
	: m_Position(local_position)
	, m_Size(local_size)
{
}

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

bool BoxCollider::DoesSegmentIntersect(const Vector& line_start, const Vector& line_end) const
{
	if (DoesContainPoint(line_start) || DoesContainPoint(line_end))
		return true;

	if (DoesSegmentIntersectBoundary(line_start, line_end))
		return true;

	return false;
}

bool BoxCollider::DoesSegmentIntersect(const Segment& seg) const
{
	return DoesSegmentIntersect(seg.start, seg.end);
}

bool BoxCollider::DoesContainPoint(const Vector& p) const
{
	const Vector box_min = GetWorldPos() - GetWorldSize() / 2.f;
	const Vector box_max = box_min + GetWorldSize();

	const bool box_contains = p.x >= box_min.x && p.x <= box_max.x
		&& p.y >= box_min.y && p.y <= box_max.y;

	if (m_InsideOutCollision)
	{
		return !box_contains;
	}
	else
	{
		return box_contains;
	}
}

bool BoxCollider::DoesSegmentIntersectBoundary(const Vector& seg_start, const Vector& seg_end) const
{
	const Segment target_seg = Segment(seg_start, seg_end);
	return DoesSegmentIntersectBoundary(target_seg);
}

bool BoxCollider::DoesSegmentIntersectBoundary(const Segment& segment) const
{
	const Boundary boundary = GetWorldBoundary();

	if (boundary.left.DoesCross(segment))
		return true;

	if (boundary.right.DoesCross(segment))
		return true;

	if (boundary.top.DoesCross(segment))
		return true;

	if (boundary.bottom.DoesCross(segment))
		return true;

	return false;
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

BoxCollider::Boundary BoxCollider::GetWorldBoundary() const
{
	const Vector top_left = m_Position - Vector(m_Size.x, m_Size.y) / 2.f;
	const Vector top_right = top_left + Vector(m_Size.x, 0);
	const Vector bottom_left = top_left + Vector(0, m_Size.y);
	const Vector bottom_right = bottom_left + Vector(m_Size.x, 0);

	const GameObject& owner = GetOwner();
	const Vector top_left_world = owner.TransformPoint(top_left);
	const Vector top_right_world = owner.TransformPoint(top_right);
	const Vector bottom_left_world = owner.TransformPoint(bottom_left);
	const Vector bottom_right_world = owner.TransformPoint(bottom_right);

	const Segment left_boundary_world = Segment(top_left_world, bottom_left_world);
	const Segment right_boundary_world = Segment(top_right_world, bottom_right_world);
	const Segment top_boundary_world = Segment(top_left_world, top_right_world);
	const Segment bottom_boundary_world = Segment(bottom_left_world, bottom_right_world);

	Boundary boundary;
	boundary.left = left_boundary_world;
	boundary.right = right_boundary_world;
	boundary.top = top_boundary_world;
	boundary.bottom = bottom_boundary_world;

	return boundary;
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

std::optional<Vector> BoxCollider::GetFirstContactPointInBoundaryWithSegment(const Vector& seg_start, const Vector& seg_end) const
{
	std::vector<Vector> contact_points;
	GetContactPointsInBoundaryWithSegment(seg_start, seg_end, contact_points);

	if (contact_points.empty())
		return std::nullopt;

	return GetClosestPointToPoint(contact_points, seg_start);
}

void BoxCollider::GetContactPointsInBoundaryWithSegment(const Vector& seg_start, const Vector& seg_end, std::vector<Vector>& out_points) const
{
	auto test_and_add_segment_contact = [&out_points, seg_start, seg_end](const Segment& seg)
	{
		const Vector crossing = seg.GetCrossingPoint(Segment(seg_start, seg_end));
		if (crossing != Vector::INVALID)
		{
			out_points.emplace_back(crossing);
		}
	};

	const Boundary boundary = GetWorldBoundary();
	test_and_add_segment_contact(boundary.top);
	test_and_add_segment_contact(boundary.bottom);
	test_and_add_segment_contact(boundary.left);
	test_and_add_segment_contact(boundary.right);
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

Vector BoxCollider::GetClosestPointToPoint(const vector<Vector>& points, const Vector& target)
{
	float min_dist_sq = INFINITY;
	Vector min_dist_p = Vector::INVALID;

	for (const Vector& p : points)
	{
		const float dist_sq = p.GetDistanceSq(target);
		if (dist_sq < min_dist_sq)
		{
			min_dist_sq = dist_sq;
			min_dist_p = p;
		}
	}

	return min_dist_p;
}

bool BoxCollider::IsLineInside(float min1, float max1, float min2, float max2)
{
	return (min1 >= min2 && max1 <= max2)
		|| (min2 >= min1 && max2 <= max1);
}

Vector BoxCollider::LinesIntersection(float min1, float max1, float min2, float max2)
{
	if (!DoLinesIntersect(min1, max1, min2, max2))
		return Vector(0, 0);

	float start = (min1 < min2) ? min2 : min1;
	float end = (max1 < max2) ? max1 : max2;
	return Vector(start, end);
}

Rect BoxCollider::GetIntersection(const BoxCollider& other) const
{
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

std::optional<Vector> BoxCollider::GetFirstContactPointWithSegment(const Vector& seg_start, const Vector& seg_end) const
{
	if (DoesContainPoint(seg_start))
		return seg_start;
	return GetFirstContactPointInBoundaryWithSegment(seg_start, seg_end);
}

bool BoxCollider::IsStatic() const
{
	return m_IsStatic;
}

void BoxCollider::GetVertices(std::vector<Vector>& out_vertices, std::vector<IndexPair>& out_edges) const
{
	const Vector top_left = m_Position - Vector(m_Size.x, m_Size.y) / 2.f;
	const Vector top_right = top_left + Vector(m_Size.x, 0);
	const Vector bottom_left = top_left + Vector(0, m_Size.y);
	const Vector bottom_right = bottom_left + Vector(m_Size.x, 0);

	const GameObject& owner = GetOwner();

	out_vertices.push_back(owner.TransformPoint(top_left));
	const size_t top_left_idx = out_vertices.size() - 1;

	out_vertices.push_back(owner.TransformPoint(top_right));
	const size_t top_right_idx = out_vertices.size() - 1;

	out_vertices.push_back(owner.TransformPoint(bottom_left));
	const size_t bottom_left_idx = out_vertices.size() - 1;

	out_vertices.push_back(owner.TransformPoint(bottom_right));
	const size_t bottom_right_idx = out_vertices.size() - 1;

	out_edges.push_back({ top_left_idx, top_right_idx });
	out_edges.push_back({ top_left_idx, bottom_left_idx });
	out_edges.push_back({ bottom_left_idx, bottom_right_idx });
	out_edges.push_back({ bottom_right_idx, top_right_idx });
}

bool BoxCollider::ShouldNavmeshIgnore() const
{
	return m_ShouldNavmeshIgnore;
}
