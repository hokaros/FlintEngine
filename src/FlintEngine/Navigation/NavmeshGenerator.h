#pragma once
#include <Core/IGameObjectContainer.h>
#include "Navmesh.h"
#include <map>
#include <Math/Segment.h>

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
		using PointPair = std::pair<Vector, Vector>;

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

		class VertexLinker
		{
		public:
			VertexLinker(const std::vector<BoxCollider*>& colliders, const std::vector<Vector>& vertices, const std::vector<IndexPair>& collider_links);

			void Link(std::vector<IndexPair>& out_links);

		private:
			void GetAllPairs(std::vector<IndexPair>& out_pairs);
			void SortByDistanceAscending(std::vector<IndexPair>& point_pairs);
			void CreateLinks(const std::vector<IndexPair>& all_pairs, std::vector<IndexPair>& out_links);

			bool IsLineOfSight(IndexPair pair) const;
			bool IsLineOfSight(const Vector& p1, const Vector& p2) const;
			bool AnyLinkInLine(const Segment& line) const;
			bool AnyColliderInLine(const Segment& line) const;

			bool IsColliderLink(IndexPair link) const;

		private:
			const std::vector<BoxCollider*>& m_Colliders;
			const std::vector<Vector>& m_Vertices;
			const std::vector<IndexPair>& m_ColliderLinks;
			std::vector<IndexPair>* m_CurrentLinks = nullptr;
		};

	private:
		NavmeshGenerator(const IGameObjectContainer& context);
		void GenerateImpl(Navmesh& navmesh);

		static void ExcludeIgnoredColliders(std::vector<BoxCollider*>& colliders);

		static void GetVertices(const std::vector<WalkableSurface*>& walkables, const std::vector<BoxCollider*>& colliders, std::vector<Vector>& out_vertices, std::vector<IndexPair>& collider_links);
		static void TransferLinksToNavmesh(const std::vector<Vector>& points, const std::vector<std::pair<size_t, size_t>>& links, Navmesh& out_navmesh);
		static void GetTrianglesFromLinks(const std::vector<std::pair<size_t, size_t>>& point_pairs, std::vector<IndexTriangle>& out_triangles);

	private:
		const IGameObjectContainer& m_Context;
	};
}