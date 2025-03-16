#pragma once
#include <vector>
#include <Math/GeometryStructures.h>
#include <Math/Vector.h>
#include <Math/Triangle.h>
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
		};

	private:
		const Vector& GetPosAtIndex(int idx) const;
		Triangle IndexTriangleToTriangle(const IndexTriangle& tri) const;
		Vector GetTriangleMid(const IndexTriangle& tri) const;
		bool IsPosInsideTriangle(const Vector& pos, const IndexTriangle& tri) const;

		void RenderTriangles(SceneRenderer& renderer) const;
		void RenderTriangle(const IndexTriangle& tri, SceneRenderer& renderer) const;
		void RenderEdges(SceneRenderer& renderer) const;
		void RenderEdge(const IndexPair& e, SceneRenderer& renderer) const;
		void RenderVertexIndices(SceneRenderer& renderer) const;
		void RenderEdgeIndices(SceneRenderer& renderer) const;

		void RenderGraph(SceneRenderer& renderer) const;

		void CreateGraph();

	private:
		std::vector<Vector> m_Vertices;
		std::vector<NavmeshTriangle> m_Triangles;
		std::vector<IndexPair> m_Edges; // TODO: remove
		graph::PositionGraph m_Graph;
	};
}