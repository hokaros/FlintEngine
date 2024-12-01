#include "NavmeshGenerator.h"

#include <iostream>
#include <algorithm>
#include <Components/WalkableSurface.h>
#include <Components/BoxCollider.h>
#include <Core/GameObject.h>
#include <FTL/VectorOperations.h>
#include <Math/Line.h>
#include <Math/Segment.h>

namespace Navigation
{
	void NavmeshGenerator::Generate(const IGameObjectContainer& context, Navmesh& navmesh)
	{
		NavmeshGenerator generator(context);
		generator.GenerateImpl(navmesh);
	}

	NavmeshGenerator::NavmeshGenerator(const IGameObjectContainer& context)
		: m_Context(context)
	{

	}

	void NavmeshGenerator::GenerateImpl(Navmesh& navmesh)
	{
		const std::vector<std::unique_ptr<GameObject>>& game_objects = m_Context.GetGameObjects();

		std::vector<WalkableSurface*> walkables;
		GameObject::FindComponentsInHierarchies(game_objects, walkables);

		std::vector<BoxCollider*> colliders;
		GameObject::FindComponentsInHierarchies(game_objects, colliders);
		ExcludeIgnoredColliders(colliders);

		std::vector<Vector> vertices;
		std::vector<IndexPair> collider_links;
		GetVertices(walkables, colliders, vertices, collider_links);

		VertexLinker linker(colliders, vertices, collider_links);
		std::vector<IndexPair> links;
		linker.Link(links);

		TransferLinksToNavmesh(vertices, links, navmesh);

		navmesh.OnNavmeshCompleted();
	}

	void NavmeshGenerator::ExcludeIgnoredColliders(std::vector<BoxCollider*>& colliders)
	{
		for (auto it = colliders.begin(); it != colliders.end();)
		{
			BoxCollider* collider = *it;
			if (collider->ShouldNavmeshIgnore())
			{
				it = colliders.erase(it);
			}
			else
			{
				it++;
			}
		}
	}

	void NavmeshGenerator::GetVertices(const std::vector<WalkableSurface*>& walkables, const std::vector<BoxCollider*>& colliders, std::vector<Vector>& out_vertices, std::vector<IndexPair>& collider_links)
	{
		for (const WalkableSurface* surf : walkables)
		{
			surf->GetVertices(out_vertices, collider_links);
		}

		for (const BoxCollider* collider : colliders)
		{
			collider->GetVertices(out_vertices, collider_links);
		}
	}

	void NavmeshGenerator::TransferLinksToNavmesh(const std::vector<Vector>& points, const std::vector<IndexPair>& links, Navmesh& out_navmesh)
	{
		out_navmesh.AddVertices(points);

		std::vector<IndexTriangle> triangles;
		GetTrianglesFromLinks(links, triangles);
		out_navmesh.AddTriangles(triangles);
		
		out_navmesh.AddEdges(links);

		// TODO: don't add triangles which are from colliders (it's a problem only if we have triangular colliders)
	}

	void NavmeshGenerator::GetTrianglesFromLinks(const std::vector<IndexPair>& point_pairs, std::vector<IndexTriangle>& out_triangles)
	{
		if (point_pairs.size() <= 2)
			return;

		NeighbouringDict neighbouring_dict;

		for (const auto& link : point_pairs)
		{
			const size_t p1 = link.first;
			const size_t p2 = link.second;

			neighbouring_dict.RegisterLink(p1, p2);

			size_t neighbour;
			if (neighbouring_dict.TryGetMutualNeighbour(p1, p2, neighbour))
			{
				out_triangles.push_back(IndexTriangle(p1, p2, neighbour));
			}
		}
	}


	void NavmeshGenerator::NeighbouringDict::RegisterLink(size_t p1, size_t p2)
	{
		RegisterNeighbour(p1, p2);
		RegisterNeighbour(p2, p1);
	}

	bool NavmeshGenerator::NeighbouringDict::TryGetMutualNeighbour(size_t p1, size_t p2, size_t& neighbour) const
	{
		auto it1 = m_PointNeighbours.find(p1);
		if (it1 == m_PointNeighbours.end())
			return false; // p1 has no neighbours

		auto it2 = m_PointNeighbours.find(p2);
		if (it2 == m_PointNeighbours.end())
			return false; // p2 has no neighbours

		return TryGetMutualPoint(it1->second, it2->second, neighbour);
	}

	void NavmeshGenerator::NeighbouringDict::RegisterNeighbour(size_t p, size_t neighbour)
	{
		auto it = m_PointNeighbours.find(p);
		if (it == m_PointNeighbours.end())
		{
			auto& emplace_res = m_PointNeighbours.emplace(p, std::vector<size_t>());
			it = emplace_res.first;
		}

		it->second.push_back(neighbour);
	}

	bool NavmeshGenerator::NeighbouringDict::TryGetMutualPoint(const std::vector<size_t>& points1, const std::vector<size_t>& points2, size_t& mutual)
	{
		for (size_t p1 : points1)
		{
			for (size_t p2 : points2)
			{
				if (p1 == p2)
				{
					mutual = p1;
					return true;
				}
			}
		}

		return false;
	}


	NavmeshGenerator::VertexLinker::VertexLinker(const std::vector<BoxCollider*>& colliders, const std::vector<Vector>& vertices, const std::vector<IndexPair>& collider_links)
		: m_Colliders(colliders)
		, m_Vertices(vertices)
		, m_ColliderLinks(collider_links)
	{
	}

	void NavmeshGenerator::VertexLinker::Link(std::vector<IndexPair>& out_links)
	{
		m_CurrentLinks = &out_links;

		std::vector<IndexPair> vertex_pairs;
		GetAllPairs(vertex_pairs);
		SortByDistanceAscending(vertex_pairs);
		CreateLinks(vertex_pairs, out_links);
	}

	void NavmeshGenerator::VertexLinker::GetAllPairs(std::vector<IndexPair>& out_pairs)
	{
		const size_t expected_size = m_Vertices.size() * (m_Vertices.size() - 1) / 2;
		out_pairs.reserve(out_pairs.size() + expected_size);

		for (size_t i = 0; i < m_Vertices.size(); i++)
		{
			for (size_t j = i + 1; j < m_Vertices.size(); j++)
			{
				out_pairs.push_back({ i, j });
			}
		}
	}

	void NavmeshGenerator::VertexLinker::SortByDistanceAscending(std::vector<IndexPair>& point_pairs)
	{
		using IndexPair = std::pair<size_t, size_t>;

		auto geom_distance_sq_from_indices = [this](const IndexPair& indices) -> float
		{
			return Vector::GetDistanceSq(m_Vertices[indices.first], m_Vertices[indices.second]);
		};

		std::sort(point_pairs.begin(), point_pairs.end(), [&geom_distance_sq_from_indices](const IndexPair& lhs, const IndexPair& rhs) -> bool
		{
			return geom_distance_sq_from_indices(lhs) < geom_distance_sq_from_indices(rhs);
		});
	}

	void NavmeshGenerator::VertexLinker::CreateLinks(const std::vector<IndexPair>& all_pairs, std::vector<IndexPair>& out_links)
	{
		for (IndexPair in_pair : all_pairs)
		{
			if (IsColliderLink(in_pair) || IsLineOfSight(in_pair)) // TODO: check if within walkable surface
			{
				out_links.push_back(in_pair);
			}
		}

		// TODO: triangle relaxation
	}

	bool NavmeshGenerator::VertexLinker::IsLineOfSight(IndexPair pair) const
	{
		const Vector p1 = m_Vertices[pair.first];
		const Vector p2 = m_Vertices[pair.second];
		return IsLineOfSight(p1, p2);
	}

	bool NavmeshGenerator::VertexLinker::IsLineOfSight(const Vector& p1, const Vector& p2) const
	{
		const Segment segment_with_tolerance = Segment(p1, p2).GetShortenedSegment(s_Tolerance);

		if (AnyLinkInLine(segment_with_tolerance))
			return false;

		if (AnyColliderInLine(segment_with_tolerance))
			return false;

		return true;
	}

	bool NavmeshGenerator::VertexLinker::AnyLinkInLine(const Segment& line) const
	{
		FE_ASSERT(m_CurrentLinks != nullptr, "No links set");

		for (IndexPair link : *m_CurrentLinks)
		{
			const Vector& link_p1 = m_Vertices[link.first];
			const Vector& link_p2 = m_Vertices[link.second];
			const Segment link_seg = Segment(link_p1, link_p2).GetShortenedSegment(s_Tolerance);
			if (line.DoesCross(link_seg))
			{
				return true;
			}
		}

		return false;
	}

	bool NavmeshGenerator::VertexLinker::AnyColliderInLine(const Segment& line) const
	{
		for (const BoxCollider* collider : m_Colliders)
		{
			if (collider->DoesSegmentIntersect(line))
			{
				return true;
			}
		}

		return false;
	}

	bool NavmeshGenerator::VertexLinker::IsColliderLink(IndexPair link) const
	{
		return ftl::vector_contains(m_ColliderLinks, link) || ftl::vector_contains(m_ColliderLinks, { link.second, link.first });
	}
}