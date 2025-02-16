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
	void NavmeshGenerator::Generate(const IGameObjectContainer& context, const NavmeshGenerationParams& params, Navmesh& navmesh)
	{
		NavmeshGenerator generator(context, params);
		generator.GenerateImpl(navmesh);
	}

	NavmeshGenerator::NavmeshGenerator(const IGameObjectContainer& context, const NavmeshGenerationParams& params)
		: m_Context(context)
		, m_Params(params)
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

		VertexCollection vertices;
		VertexCollection::EdgeCollectionId collider_links_id;
		GetVertices(walkables, colliders, vertices, collider_links_id);

		VertexLinker linker(colliders, vertices.GetVertices(), vertices.GetEdgeCollection(collider_links_id));
		std::vector<IndexPair> links;
		linker.Link(links);

		TransferLinksToNavmesh(vertices.GetVertices(), links, navmesh);

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

	void NavmeshGenerator::GetVertices(const std::vector<WalkableSurface*>& walkables, const std::vector<BoxCollider*>& colliders, VertexCollection& out_vertices, VertexCollection::EdgeCollectionId& out_collider_links_id)
	{
		out_collider_links_id = out_vertices.CreateEdgeCollection();
		std::vector<IndexPair>& collider_links = out_vertices.GetEdgeCollection(out_collider_links_id);

		std::vector<Vector>& underlying_vertices = out_vertices.GetVertices();

		for (const WalkableSurface* surf : walkables)
		{
			surf->GetVertices(underlying_vertices, collider_links);
		}

		for (const BoxCollider* collider : colliders)
		{
			collider->GetVertices(underlying_vertices, collider_links);
		}

		MergeColliderVertices(out_vertices, collider_links);
		CutColliderLinks(out_vertices, collider_links);
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

		for (const IndexPair& link : point_pairs)
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

	void NavmeshGenerator::CutColliderLinks(VertexCollection& vertices, std::vector<IndexPair>& collider_links)
	{
		size_t new_collider_links_offset = 0;

		size_t i = 0;
		while (true)
		{
			const size_t new_new_collider_links_offset = CutColliderLinksForNew(vertices, collider_links, new_collider_links_offset);
			if (new_new_collider_links_offset == new_collider_links_offset)
				break;

			new_collider_links_offset = new_new_collider_links_offset;
			i++;

			if (i > 1000)
			{
				FE_WARN("Cutting collider links took too long. Skipping the rest");
				break;
			}
		}
	}

	size_t NavmeshGenerator::CutColliderLinksForNew(VertexCollection& vertices, std::vector<IndexPair>& collider_links, size_t new_collider_links_offset)
	{
		size_t newest_collider_links_offset = collider_links.size();

		// Only new collider links with every other
		for (size_t new_i = new_collider_links_offset; new_i < newest_collider_links_offset; new_i++)
		{
			for (size_t all_i = 0; all_i < newest_collider_links_offset && all_i < new_i; all_i++)
			{
				TryCutColliderLinksIJ(vertices, collider_links, new_i, all_i, newest_collider_links_offset);
			}
		}

		return newest_collider_links_offset;
	}

	void NavmeshGenerator::TryCutColliderLinksIJ(VertexCollection& vertices, std::vector<IndexPair>& collider_links, size_t& i, size_t& j, size_t& new_collider_links_offset)
	{
		const IndexPair link1 = collider_links[i];
		const IndexPair link2 = collider_links[j];

		const Segment seg1 = IndexPairToSegment(link1, vertices.GetVertices());
		const Segment seg2 = IndexPairToSegment(link2, vertices.GetVertices());

		if (seg1.GetLengthSq() == 0.f || seg2.GetLengthSq() == 0.f)
			return;

		if (seg1.IsSameDirection(seg2))
			return; // TODO: should we handle it?

		const Vector crossing_point = seg1.GetCrossingPoint(seg2);
		if (crossing_point == Vector::INVALID)
			return;

		Vector crossing_at_segment_end;
		const bool seg1_contains_crossing = TryGetEqualSegmentEnd(seg1, crossing_point, crossing_at_segment_end);
		const bool seg2_contains_crossing = TryGetEqualSegmentEnd(seg2, crossing_point, crossing_at_segment_end);
		if (seg1_contains_crossing && seg2_contains_crossing)
			return;

		if (seg1_contains_crossing || seg2_contains_crossing)
		{ // One segment is not cut, there are no new vertices
			size_t crossing_point_index = 0;
			size_t cut_link_index = 0;

			// Check the configuration in which to cut
			if (vertices[link1.first] == crossing_at_segment_end)
			{
				crossing_point_index = link1.first;
				cut_link_index = j;
			}
			else if (vertices[link1.second] == crossing_at_segment_end)
			{
				crossing_point_index = link1.second;
				cut_link_index = j;
			}
			else if (vertices[link2.first] == crossing_at_segment_end)
			{
				crossing_point_index = link2.first;
				cut_link_index = i;
			}
			else
			{
				crossing_point_index = link2.second;
				cut_link_index = i;
			}

			// Perform the cut 
			const IndexPair cut_link = collider_links[cut_link_index];
			collider_links.push_back(IndexPair(cut_link.first, crossing_point_index));
			collider_links.push_back(IndexPair(cut_link.second, crossing_point_index));
			collider_links.erase(collider_links.begin() + cut_link_index);

			if (i >= cut_link_index)
			{
				i--;
			}
			if (j >= cut_link_index)
			{
				j--;
			}

			new_collider_links_offset--;
		}
		else
		{
			const size_t crossing_point_index = vertices.AddVertex(crossing_point);

			// Add new collider links
			collider_links.push_back(IndexPair(link1.first, crossing_point_index));
			collider_links.push_back(IndexPair(link1.second, crossing_point_index));
			collider_links.push_back(IndexPair(link2.first, crossing_point_index));
			collider_links.push_back(IndexPair(link2.second, crossing_point_index));

			// Remove old collider links
			collider_links.erase(collider_links.begin() + i);
			if (i > 0)
			{
				i--;
			}
			if (j >= i)
			{
				j--;
			}
			collider_links.erase(collider_links.begin() + j);
			if (j > 0)
			{
				j--;
			}
			if (i >= j)
			{
				i--;
			}

			new_collider_links_offset -= 2;
		}
	}

	void NavmeshGenerator::MergeColliderVertices(VertexCollection& vertices, std::vector<IndexPair>& collider_links)
	{
		for (size_t i = 0; i < vertices.GetNumVertices(); i++)
		{
			for (size_t j = i + 1; j < vertices.GetNumVertices(); j++)
			{
				if (j <= i)
					continue;

				const Vector& v1 = vertices[i];
				const Vector& v2 = vertices[j];

				// TODO: what about groups when v_i should be merged with v_x but v_j shouldn't?
				const float merge_dist = m_Params.merge_distance;
				if ((v1 - v2).LengthSquared() < merge_dist * merge_dist)
				{
					const Vector merged_vertex = (v1 + v2) / 2.0f;
					vertices.MergeVertices(i, j, merged_vertex);
				}
			}
		}
	}

	Segment NavmeshGenerator::IndexPairToSegment(IndexPair index_pair, const std::vector<Vector>& vertices)
	{
		const Vector start = vertices[index_pair.first];
		const Vector end = vertices[index_pair.second];
		return Segment(start, end);
	}

	bool NavmeshGenerator::TryGetEqualSegmentEnd(const Segment& segment, const Vector& desired_end_pos, Vector& end_pos)
	{
		if (segment.start == desired_end_pos)
		{
			end_pos = segment.start;
			return true;
		}
		if (segment.end == desired_end_pos)
		{
			end_pos = segment.end;
			return true;
		}

		return false;
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
			if (link_seg.GetLengthSq() == 0.f)
				continue;

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


	bool NavmeshGenerationParams::operator!=(const NavmeshGenerationParams& other) const
	{
		return merge_distance != other.merge_distance;
	}
}