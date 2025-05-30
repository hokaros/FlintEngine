#include "NavmeshGenerator.h"

#include <iostream>
#include <algorithm>
#include <unordered_map>

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

		TransferLinksToNavmesh(vertices.GetVertices(), std::move(links), navmesh);

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

		MergeVertices(out_vertices);
		QuantizeVertices(out_vertices);
		MergeDuplicateLinks(collider_links);
		CutColliderLinks(out_vertices, collider_links);
		RemoveVerticesOutsideWalkableSurfaces(out_vertices, walkables);
	}

	void NavmeshGenerator::TransferLinksToNavmesh(const std::vector<Vector>& points, std::vector<IndexPair>&& links, Navmesh& out_navmesh)
	{
		out_navmesh.AddVertices(points);

		std::vector<IndexTriangle> triangles;
		GetTrianglesFromLinks(links, triangles);
		out_navmesh.AddTriangles(triangles);

		//RemoveLinksNotInTriangles(links, triangles);

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

			std::vector<size_t> mutual_neighbours;
			neighbouring_dict.GetMutualNeighbours(p1, p2, mutual_neighbours);

			FE_ASSERT(mutual_neighbours.size() <= 2, "Planar points cannot have more than 2 neighbours");
			
			for (size_t mutual_neighbour : mutual_neighbours)
			{
				out_triangles.push_back(IndexTriangle(p1, p2, mutual_neighbour));
			}
		}
	}

	void NavmeshGenerator::GetLinksNotInTriangles(const std::vector<IndexPair>& links, const std::vector<IndexTriangle>& triangles, std::vector<IndexPair>& not_in_triangles)
	{
		for (IndexPair link : links)
		{
			if (!IsLinkInTriangles(link, triangles))
			{
				not_in_triangles.push_back(link);
			}
		}
	}

	void NavmeshGenerator::RemoveVerticesOutsideWalkableSurfaces(VertexCollection& vertices, const std::vector<WalkableSurface*>& walkables)
	{
		for (size_t i = 0; i < vertices.GetNumVertices();)
		{
			const Vector& vertex = vertices[i];
			
			if (!AnyWalkableSurfaceContainsPoint(vertex, walkables))
			{
				vertices.RemoveVertex(i);
			}
			else
			{
				i++;
			}
		}
	}

	bool NavmeshGenerator::AnyWalkableSurfaceContainsPoint(const Vector& v, const std::vector<WalkableSurface*>& walkables)
	{
		for (const WalkableSurface* walkable : walkables)
		{
			if (walkable->ContainsPoint(v, s_Tolerance))
			{
				return true;
			}
		}

		return false;
	}

	void NavmeshGenerator::CutColliderLinks(VertexCollection& vertices, std::vector<IndexPair>& collider_links)
	{
		size_t new_collider_links_offset = 0;

		size_t i = 0;
		while (true)
		{
			const size_t prev_collider_links_count = collider_links.size();
			const size_t new_new_collider_links_offset = CutColliderLinksForNew(vertices, collider_links, new_collider_links_offset);
			if (prev_collider_links_count == collider_links.size() && new_new_collider_links_offset == new_collider_links_offset)
				break;

			new_collider_links_offset = new_new_collider_links_offset;

			MergeDuplicateLinksForNew(collider_links, new_collider_links_offset);

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
		const IndexPair orig_link1 = collider_links[i];
		const IndexPair orig_link2 = collider_links[j];

		const Segment orig_seg1 = IndexPairToSegment(orig_link1, vertices.GetVertices());
		const Segment orig_seg2 = IndexPairToSegment(orig_link2, vertices.GetVertices());

		/*auto is_almost = [](float actual, float expected) -> bool
		{
			return fabsf(actual - expected) < 0.3f;
		};

		auto is_almost_vec = [&is_almost](const Vector& actual, const Vector& expected) -> bool
		{
			return is_almost(actual.x, expected.x) && is_almost(actual.y, expected.y);
		};

		auto does_segment_end_match = [&is_almost_vec](const Segment& seg, const Vector& expected_end) -> bool
		{
			return is_almost_vec(seg.start, expected_end) || is_almost_vec(seg.end, expected_end);
		};

		auto does_segment_match = [&does_segment_end_match](const Segment& seg) -> bool
		{
			return does_segment_end_match(seg, Vector(710, 10)) && does_segment_end_match(seg, Vector(10, 10));
		};

		if (does_segment_match(orig_seg1) || does_segment_match(orig_seg2))
		{
			int x = 1;
		}*/

		std::vector<Segment> cut_result = orig_seg1.CutWith(orig_seg2);

		if (cut_result.size() == 2 && ftl::vector_contains(cut_result, orig_seg1) && ftl::vector_contains(cut_result, orig_seg2) && orig_seg1 != orig_seg2)
			return;

		// TODO: extract a class
		using VertexMapT = std::map<Vector, size_t, VectorTreeComparator>;
		VertexMapT segment_vertices_to_indices; // position of the vertex -> index of the vertex
		auto dict_contains_vertex = [](const VertexMapT& dict, const Vector& vertex)
		{
			return dict.find(vertex) != dict.end();
		};
		auto insert_vertex_at_index_to_dict = [&](VertexMapT& dict, size_t vertex_index)
		{
			const Vector& vertex_pos = vertices[vertex_index];
			if (dict_contains_vertex(dict, vertex_pos))
				return;

			dict.insert({ vertex_pos, vertex_index });
		};

		// Original vertices
		insert_vertex_at_index_to_dict(segment_vertices_to_indices, orig_link1.first);
		insert_vertex_at_index_to_dict(segment_vertices_to_indices, orig_link1.second);
		insert_vertex_at_index_to_dict(segment_vertices_to_indices, orig_link2.first);
		insert_vertex_at_index_to_dict(segment_vertices_to_indices, orig_link2.second);

		auto add_vertex_if_absent = [&](const Vector& vertex)
		{
			if (!dict_contains_vertex(segment_vertices_to_indices, vertex))
			{
				const size_t vertex_index = vertices.AddVertex(vertex);
				insert_vertex_at_index_to_dict(segment_vertices_to_indices, vertex_index);
			}
		};
		
		// Add new segments
		bool is_orig_seg1_present = false;
		bool is_orig_seg2_present = false;
		for (const Segment& new_seg : cut_result)
		{
			if (new_seg == orig_seg1)
			{
				is_orig_seg1_present = true;

				FE_ASSERT(new_seg != orig_seg2, "New segment is equal to 2 original ones");
			}
			else if (new_seg == orig_seg2)
			{
				is_orig_seg2_present = true;
			}
			else
			{ // New segment is new
				add_vertex_if_absent(new_seg.start);
				add_vertex_if_absent(new_seg.end);

				const size_t seg_start_index = segment_vertices_to_indices[new_seg.start];
				const size_t seg_end_index = segment_vertices_to_indices[new_seg.end];

				collider_links.push_back(IndexPair(seg_start_index, seg_end_index));
			}
		}

		// Remove the ones that are no longer present
		if (!is_orig_seg1_present)
		{
			collider_links.erase(collider_links.begin() + i);
			new_collider_links_offset--;
			i--;

			if (j >= i)
			{
				j--;
			}
		}

		if (!is_orig_seg2_present)
		{
			collider_links.erase(collider_links.begin() + j);
			new_collider_links_offset--;
			j--;

			if (i >= j)
			{
				i--;
			}
		}
	}

	void NavmeshGenerator::MergeDuplicateLinksForNew(std::vector<IndexPair>& links, size_t new_links_offset)
	{
		// Only new links with every other
		for (size_t all_i = 0; all_i < links.size(); all_i++)
		{
			size_t new_i = all_i >= new_links_offset ? all_i + 1 : new_links_offset; // Don't duplicate checks between new links
			for (; new_i < links.size();)
			{
				const IndexPair& link_from_new = links[new_i];
				const IndexPair& link_from_all = links[all_i];

				if (link_from_new == link_from_all)
				{
					links.erase(links.begin() + new_i);
				}
				else
				{
					new_i++;
				}
			}
		}
	}

	void NavmeshGenerator::MergeVertices(VertexCollection& vertices)
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

	void NavmeshGenerator::MergeDuplicateLinks(std::vector<IndexPair>& collider_links)
	{
		for (auto it1 = collider_links.begin(); it1 != collider_links.end(); it1++)
		{
			for (auto it2 = it1 + 1; it2 != collider_links.end();)
			{
				const IndexPair& link1 = *it1;
				const IndexPair& link2 = *it2;

				if (link1 == link2)
				{
					it2 = collider_links.erase(it2);
				}
				else
				{
					it2++;
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

	bool NavmeshGenerator::IsLinkInTriangles(IndexPair link, const std::vector<IndexTriangle>& triangles)
	{
		for (const IndexTriangle& tri : triangles)
		{
			if (tri.ContainsIndex(link.first) && tri.ContainsIndex(link.second))
			{
				return true;
			}
		}

		return false;
	}

	void NavmeshGenerator::QuantizeVertices(VertexCollection& vertices)
	{
		for (size_t i = 0; i < vertices.GetNumVertices(); i++)
		{
			vertices[i] = QuantizeVertex(vertices[i]);
		}
	}

	Vector NavmeshGenerator::QuantizeVertex(const Vector& v)
	{
		return Vector(QuantizeVertexComponent(v.x), QuantizeVertexComponent(v.y));
	}

	float NavmeshGenerator::QuantizeVertexComponent(float v)
	{
		constexpr float quant = s_Tolerance * 2.0f;

		const float remainder = fmodf(v, quant);
		if (remainder < quant / 2.0f)
		{
			return v - remainder;
		}
		else
		{
			return (v - remainder) + quant;
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

	void NavmeshGenerator::NeighbouringDict::GetMutualNeighbours(size_t p1, size_t p2, std::vector<size_t>& out_neighbours) const
	{
		auto it1 = m_PointNeighbours.find(p1);
		if (it1 == m_PointNeighbours.end())
			return; // p1 has no neighbours

		auto it2 = m_PointNeighbours.find(p2);
		if (it2 == m_PointNeighbours.end())
			return; // p2 has no neighbours

		GetMutualPoints(it1->second, it2->second, out_neighbours);
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

	void NavmeshGenerator::NeighbouringDict::GetMutualPoints(const std::vector<size_t>& points1, const std::vector<size_t>& points2, std::vector<size_t>& out_mutuals)
	{
		for (size_t p1 : points1)
		{
			for (size_t p2 : points2)
			{
				if (p1 == p2)
				{
					out_mutuals.push_back(p1);
				}
			}
		}
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
		//size_t stop_at = 218; // find the last that doesn't create the link (it's the first that would)

		//size_t i = 0;
		for (IndexPair in_pair : all_pairs)
		{
			/*if (i >= stop_at)
			{
				break;
			}*/

			if (IsColliderLink(in_pair) || IsLineOfSight(in_pair)) // TODO: check if within walkable surface
			{
				out_links.push_back(in_pair);
			}

			//i++;
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