#pragma once
#include "ComponentDefinition.h"
#include <Math/GeometryStructures.h>
#include <Math/Segment.h>

class BoxCollider
	: public ObjectComponent
{
	DECLARE_COMPONENT();

public:
	BoxCollider() = default;
	BoxCollider(Vector local_position, Vector local_size);

	bool DoesCollide(const BoxCollider& other) const;
	bool DoesSegmentIntersect(const Vector& line_start, const Vector& line_end) const;
	bool DoesSegmentIntersect(const Segment& seg) const;
	bool DoesContainPoint(const Vector& p) const;
	bool DoesSegmentIntersectBoundary(const Vector& seg_start, const Vector& seg_end) const;
	bool DoesSegmentIntersectBoundary(const Segment& segment) const;
	Rect GetIntersection(const BoxCollider& other) const;
	bool IsStatic() const;
	void GetVertices(std::vector<Vector>& out_vertices, std::vector<IndexPair>& out_edges) const;

	bool ShouldNavmeshIgnore() const;

	void OnCollision(BoxCollider& other);

	void SetSize(const Vector& size);

	Vector GetWorldPos() const;
	Vector GetWorldMiddle() const;
	Vector GetWorldSize() const;

	virtual void Awake() override;
	virtual void OnDestroy() override;

public:
	std::function<void(BoxCollider& collider)> onCollision;

private:
	bool DoesIntersect(const BoxCollider& other) const;
	static bool IsInside(const BoxCollider& collider1, const BoxCollider& collider2);

	/***** Operacje na jednowymiarowych liniach ******/
	static bool DoLinesIntersect(float min1, float max1, float min2, float max2);
	static bool IsLineInside(float min1, float max1, float min2, float max2);
	static Vector LinesIntersection(float min1, float max1, float min2, float max2);

private:
	bool m_InsideOutCollision = false;
	DECLARE_FIELD(m_InsideOutCollision);

	Vector m_Position = Vector(0,0);
	DECLARE_FIELD(m_Position);

	Vector m_Size = Vector(0,0);
	DECLARE_FIELD(m_Size);

	bool m_IsStatic = false;
	DECLARE_FIELD(m_IsStatic);

	bool m_ShouldNavmeshIgnore = false;
	DECLARE_FIELD(m_ShouldNavmeshIgnore);
};

