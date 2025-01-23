#pragma once
#include <Core/IGameObjectContainer.h>
#include "Navmesh.h"
#include <map>
#include <Math/Segment.h>
#include "VertexCollection.h"

class Scene;
class WalkableSurface;
class BoxCollider;

namespace Navigation
{
	struct NavmeshGenerationParams
	{
		float merge_distance = 0.05f;

		bool operator !=(const NavmeshGenerationParams& other) const;
	};

	class NavmeshGenerator
	{
	public:
		static void Generate(const IGameObjectContainer& context, const NavmeshGenerationParams& params, Navmesh& navmesh);

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
		NavmeshGenerator(const IGameObjectContainer& context, const NavmeshGenerationParams& params);
		void GenerateImpl(Navmesh& navmesh);

		static void ExcludeIgnoredColliders(std::vector<BoxCollider*>& colliders);

		void GetVertices(const std::vector<WalkableSurface*>& walkables, const std::vector<BoxCollider*>& colliders, std::vector<Vector>& out_vertices, std::vector<IndexPair>& collider_links);
		static void TransferLinksToNavmesh(const std::vector<Vector>& points, const std::vector<IndexPair>& links, Navmesh& out_navmesh);
		static void GetTrianglesFromLinks(const std::vector<IndexPair>& point_pairs, std::vector<IndexTriangle>& out_triangles);

		static void CutColliderLinks(VertexCollection& vertices, std::vector<IndexPair>& collider_links);
		// Returns new new collider links offset
		static size_t CutColliderLinksForNew(VertexCollection& vertices, std::vector<IndexPair>& collider_links, size_t new_collider_links_offset);
		static void TryCutColliderLinksIJ(VertexCollection& vertices, std::vector<IndexPair>& collider_links, size_t& i, size_t& j, size_t& new_collider_links_offset);

		void MergeColliderVertices(VertexCollection& vertices, std::vector<IndexPair>& collider_links);

		static Segment IndexPairToSegment(IndexPair index_pair, const std::vector<Vector>& vertices);
		static bool TryGetEqualSegmentEnd(const Segment& segment, const Vector& desired_end_pos, Vector& end_pos);

	private:
		const IGameObjectContainer& m_Context;
		const NavmeshGenerationParams m_Params;

		static constexpr float s_Tolerance = 0.001f;
	};
}