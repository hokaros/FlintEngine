#pragma once
#include <Core/IGameObjectContainer.h>
#include "Navmesh.h"
#include <map>

class Scene;
class WalkableSurface;
class BoxCollider;

namespace Navigation
{
	class NavmeshGenerator
	{
	public:
		static void Generate(const IGameObjectContainer& context, Navmesh& navmesh);

	private:
		class NeighbouringDict
		{
		public:
			void RegisterLink(size_t p1, size_t p2);
			bool TryGetMutualNeighbour(size_t p1, size_t p2, size_t& neighbour) const;

		private:
			void RegisterNeighbour(size_t p, size_t neighbour);
			static bool TryGetMutualPoint(const std::vector<size_t>& points1, const std::vector<size_t>& points2, size_t& mutual);

		private:
			std::map<size_t, std::vector<size_t>> m_PointNeighbours;
		};

	private:
		NavmeshGenerator(const IGameObjectContainer& context);
		void GenerateImpl(Navmesh& navmesh);

		static void GetVertices(const std::vector<WalkableSurface*>& walkables, const std::vector<BoxCollider*>& colliders, std::vector<Vector>& out_vertices);
		static void GetAllPairs(const std::vector<Vector>& in_points, std::vector<std::pair<size_t, size_t>>& out_pairs);
		static void SortByDistanceAscending(const std::vector<Vector>& points, std::vector<std::pair<size_t, size_t>>& point_pairs);
		static void CreateLinks(const std::vector<Vector>& points, std::vector<std::pair<size_t, size_t>>& point_pairs);
		static void TransferLinksToNavmesh(const std::vector<Vector>& points, const std::vector<std::pair<size_t, size_t>>& links, Navmesh& out_navmesh);
		static void GetTrianglesFromLinks(const std::vector<std::pair<size_t, size_t>>& point_pairs, std::vector<IndexTriangle>& out_triangles);

	private:
		const IGameObjectContainer& m_Context;
	};
}