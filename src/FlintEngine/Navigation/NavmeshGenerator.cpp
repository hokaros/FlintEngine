#include "NavmeshGenerator.h"

#include <iostream>
#include <algorithm>
#include <Components/WalkableSurface.h>
#include <Components/BoxCollider.h>
#include <Core/GameObject.h>

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

		std::vector<Vector> vertices;
		GetVertices(walkables, colliders, vertices);

		std::vector<std::pair<size_t, size_t>> vertex_pairs;
		GetAllPairs(vertices, vertex_pairs);
		SortByDistanceAscending(vertices, vertex_pairs);
		CreateLinks(vertices, vertex_pairs);

		TransferLinksToNavmesh(vertices, vertex_pairs, navmesh);
	}

	void NavmeshGenerator::GetVertices(const std::vector<WalkableSurface*>& walkables, const std::vector<BoxCollider*>& colliders, std::vector<Vector>& out_vertices)
	{
		for (const WalkableSurface* surf : walkables)
		{
			surf->GetVertices(out_vertices);
		}

		for (const BoxCollider* collider : colliders)
		{
			// TODO: check if is present in any WalkableSurface
			collider->GetVertices(out_vertices);
		}
	}

	void NavmeshGenerator::GetAllPairs(const std::vector<Vector>& in_points, std::vector<std::pair<size_t, size_t>>& out_pairs)
	{
		const size_t expected_size = in_points.size() * (in_points.size() - 1) / 2;
		out_pairs.reserve(out_pairs.size() + expected_size);

		for (size_t i = 0; i < in_points.size(); i++)
		{
			for (size_t j = i + 1; j < in_points.size(); j++)
			{
				// TODO: check if is line of sight
				out_pairs.push_back({ i, j });
			}
		}
	}

	void NavmeshGenerator::SortByDistanceAscending(const std::vector<Vector>& points, std::vector<std::pair<size_t, size_t>>& point_pairs)
	{
		using IndexPair = std::pair<size_t, size_t>;

		auto geom_distance_sq_from_indices = [&points](const IndexPair& indices) -> float
		{
			return Vector::GetDistanceSq(points[indices.first], points[indices.second]);
		};

		std::sort(point_pairs.begin(), point_pairs.end(), [&geom_distance_sq_from_indices](const IndexPair& lhs, const IndexPair& rhs) -> bool
		{
			return geom_distance_sq_from_indices(lhs) < geom_distance_sq_from_indices(rhs);
		});
	}

	void NavmeshGenerator::CreateLinks(const std::vector<Vector>& points, std::vector<std::pair<size_t, size_t>>& point_pairs)
	{
		// for each pair, starting from the closest: link
		// break if an iteration didn't change anything
	}

	void NavmeshGenerator::TransferLinksToNavmesh(const std::vector<Vector>& points, const std::vector<std::pair<size_t, size_t>>& links, Navmesh& out_navmesh)
	{
		out_navmesh.AddVertices(points);

		std::vector<IndexTriangle> triangles;
		GetTrianglesFromLinks(links, triangles);
		out_navmesh.AddTriangles(triangles);
	}

	void NavmeshGenerator::GetTrianglesFromLinks(const std::vector<std::pair<size_t, size_t>>& point_pairs, std::vector<IndexTriangle>& out_triangles)
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
			auto& emplace_res = m_PointNeighbours.emplace();
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
}