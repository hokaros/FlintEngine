#pragma once
#include <vector>
#include <optional>
#include <map>
#include <Math/GeometryStructures.h>
#include <Math/Vector.h>
#include <Math/Triangle.h>
#include <Math/Segment.h>
#include <Math/DirectedRect.h>
#include <Graph/PositionGraph.h>

class SceneRenderer;

namespace Navigation
{
	class Navmesh
	{
	public:
		void Render(SceneRenderer& renderer) const;

		const graph::PositionGraph& GetGraph() const;
		graph::NodeId GetTriangleOfPos(const Vector& pos) const;

		bool ContainsPoint(const Vector& pos) const;
		bool ContainsLine(const Segment& line) const;
		bool ContainsDirectedRect(const DirectedRect& rect) const;

		void Clear();
		void AddVertex(Vector&& v);
		void AddVertices(const std::vector<Vector>& vertices);
		void AddTriangle(IndexTriangle&& tri);
		void AddTriangles(const std::vector<IndexTriangle>& triangles);
		void AddEdges(const std::vector<IndexPair>& edges);

		void OnNavmeshCompleted();

	private:
		struct NavmeshTriangle
		{
			IndexTriangle vertices;
			graph::NodeId graph_node;

			void GetEdgesAsIndexPairs(std::vector<IndexPair>& out_edges) const;
		};

		struct TriangleId
		{
			size_t index = 0;

			static const TriangleId INVALID;
		};

	private:
		const Vector& GetPosAtIndex(int idx) const;
		Triangle IndexTriangleToTriangle(const IndexTriangle& tri) const;
		Vector GetTriangleMid(const IndexTriangle& tri) const;
		bool IsPosInsideTriangle(const Vector& pos, const IndexTriangle& tri) const;

		bool DoesLineCrossNonNeighbouringTriangles(const Segment& line) const;
		bool DoesDirectedRectCrossNonNeighbouringTriangles(const DirectedRect& rect) const;

		void GetCrossedEdgesOfTriangle(const Segment& crossing_seg, const NavmeshTriangle& tri, std::vector<IndexPair>& out_crossed_edges) const;
		void GetCrossedEdgesOfTriangle(const DirectedRect& crossing_rect, const NavmeshTriangle& tri, std::vector<IndexPair>& out_crossed_edges) const;
		graph::NodeId GetTriangleNeighbourContainingEdge(const NavmeshTriangle& tri, const IndexPair& edge) const;

		TriangleId FindTriangleOfGraphNode(graph::NodeId node) const;
		const NavmeshTriangle& GetTriangleById(TriangleId id) const;

		void RenderTriangles(SceneRenderer& renderer) const;
		void RenderTriangle(const IndexTriangle& tri, SceneRenderer& renderer) const;
		void RenderEdges(SceneRenderer& renderer) const;
		void RenderEdge(const IndexPair& e, SceneRenderer& renderer) const;
		void RenderVertexIndices(SceneRenderer& renderer) const;
		void RenderEdgeIndices(SceneRenderer& renderer) const;

		void RenderGraph(SceneRenderer& renderer) const;

		void CreateGraph();

		bool DoesAnyTriangleContainPos(const std::vector<NavmeshTriangle>& triangles, const Vector& pos) const;

	private:
		std::vector<Vector> m_Vertices;
		std::vector<NavmeshTriangle> m_Triangles;
		std::vector<IndexPair> m_Edges; // TODO: remove
		graph::PositionGraph m_Graph;
		std::map<graph::NodeId, TriangleId> m_GraphNodesToTriangles;
	};
}