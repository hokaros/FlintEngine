#include "Navmesh.h"

#include <SceneRenderer.h>
#include <utility.h>

#include <string>

namespace Navigation
{
	// TODO: remove Navigation:: prefixes
	const Navigation::Navmesh::TriangleId Navigation::Navmesh::TriangleId::INVALID{ std::numeric_limits<size_t>::max() };

	void Navigation::Navmesh::Render(SceneRenderer& renderer) const
	{
		RenderVertexIndices(renderer);
		RenderEdges(renderer);
		RenderTriangles(renderer);
		//RenderEdgeIndices(renderer);
		//RenderGraph(renderer);
	}

	const graph::PositionGraph& Navigation::Navmesh::GetGraph() const
	{
		return m_Graph;
	}

	graph::NodeId Navigation::Navmesh::GetTriangleOfPos(const Vector& pos) const
	{
		for (const NavmeshTriangle& tri : m_Triangles)
		{
			if (IsPosInsideTriangle(pos, tri.vertices))
			{
				return tri.graph_node;
			}
		}

		return graph::NodeId::INVALID;
	}

	bool Navigation::Navmesh::ContainsLine(const Segment& line) const
	{
		if (!ContainsPoint(line.start) || !ContainsPoint(line.end))
			return false;

		return !DoesLineCrossNonNeighbouringTriangles(line);
	}

	bool Navigation::Navmesh::ContainsPoint(const Vector& pos) const
	{
		return DoesAnyTriangleContainPos(m_Triangles, pos);
	}

	void Navigation::Navmesh::Clear()
	{
		m_Edges.clear();
		m_Vertices.clear();
		m_Triangles.clear();
		m_Graph = {};
	}

	void Navigation::Navmesh::AddVertex(Vector&& v)
	{
		m_Vertices.push_back(std::move(v));
	}

	void Navigation::Navmesh::AddVertices(const std::vector<Vector>& vertices)
	{
		for (const Vector& v : vertices)
		{
			m_Vertices.push_back(v);
		}
	}

	void Navigation::Navmesh::AddTriangle(IndexTriangle&& tri)
	{
		m_Triangles.push_back({ std::move(tri), graph::NodeId::INVALID });
	}

	void Navigation::Navmesh::AddTriangles(const std::vector<IndexTriangle>& triangles)
	{
		for (const IndexTriangle& tri : triangles)
		{
			m_Triangles.push_back({ tri, graph::NodeId::INVALID });
		}
	}

	void Navigation::Navmesh::AddEdges(const std::vector<IndexPair>& edges)
	{
		for (const IndexPair& e : edges)
		{
			m_Edges.push_back(e);
		}
	}

	void Navigation::Navmesh::OnNavmeshCompleted()
	{
		CreateGraph();
	}

	const Vector& Navigation::Navmesh::GetPosAtIndex(int idx) const
	{
		FE_ASSERT(idx >= 0 && idx < m_Vertices.size(), "Index out of bounds");

		return m_Vertices[idx];
	}

	Triangle Navigation::Navmesh::IndexTriangleToTriangle(const IndexTriangle& tri) const
	{
		return Triangle(GetPosAtIndex(tri.idx1), GetPosAtIndex(tri.idx2), GetPosAtIndex(tri.idx3));
	}

	Vector Navigation::Navmesh::GetTriangleMid(const IndexTriangle& tri) const
	{
		const Vector& v1 = GetPosAtIndex(tri.idx1);
		const Vector& v2 = GetPosAtIndex(tri.idx2);
		const Vector& v3 = GetPosAtIndex(tri.idx3);

		return (v1 + v2 + v3) / 3.0f;
	}

	bool Navigation::Navmesh::IsPosInsideTriangle(const Vector& pos, const IndexTriangle& tri) const
	{
		return IndexTriangleToTriangle(tri).Contains(pos);
	}

	bool Navigation::Navmesh::DoesLineCrossNonNeighbouringTriangles(const Segment& line) const
	{
		// TODO: space partitioning (check only triangles which are within extents of the line)
		for (const NavmeshTriangle& tri : m_Triangles)
		{
			const std::optional<IndexPair> crossed_edge = GetCrossedEdgeOfTriangle(line, tri);
			if (crossed_edge.has_value())
			{
				const graph::NodeId neighbour_containing_edge_id = GetTriangleNeighbourContainingEdge(tri, *crossed_edge);
				if (neighbour_containing_edge_id == graph::NodeId::INVALID)
				{
					return true;
				}
			}
		}

		return false;
	}

	std::optional<IndexPair> Navmesh::GetCrossedEdgeOfTriangle(const Segment& crossing_seg, const NavmeshTriangle& tri) const
	{
		std::vector<IndexPair> edges;
		tri.GetEdgesAsIndexPairs(edges);

		for (const IndexPair& edge : edges)
		{
			const Segment edge_line = Segment(m_Vertices[edge.first], m_Vertices[edge.second]);
			if (edge_line.DoesCross(crossing_seg))
			{
				return edge;
			}
		}

		return std::nullopt;
	}

	graph::NodeId Navmesh::GetTriangleNeighbourContainingEdge(const NavmeshTriangle& tri, const IndexPair& edge) const
	{
		const graph::PositionNode* my_node = m_Graph.GetNodeById(tri.graph_node);
		for (const graph::NodeId neighbour_id : my_node->GetNeighbours())
		{
			FE_ASSERT(neighbour_id != graph::NodeId::INVALID, "Neighbour id shouldn't be invalid");
			const NavmeshTriangle& triangle = GetTriangleById(FindTriangleOfGraphNode(neighbour_id));
			if (triangle.vertices.ContainsIndexPair(edge))
			{
				return neighbour_id;
			}
		}

		return graph::NodeId::INVALID;
	}

	Navigation::Navmesh::TriangleId Navigation::Navmesh::FindTriangleOfGraphNode(graph::NodeId node) const
	{
		auto it = m_GraphNodesToTriangles.find(node);
		if (it == m_GraphNodesToTriangles.end())
			return TriangleId::INVALID;

		return it->second;
	}

	const Navigation::Navmesh::NavmeshTriangle& Navigation::Navmesh::GetTriangleById(TriangleId id) const
	{
		return m_Triangles[id.index];
	}

	void Navigation::Navmesh::RenderTriangles(SceneRenderer& renderer) const
	{
		for (const NavmeshTriangle& tri : m_Triangles)
		{
			RenderTriangle(tri.vertices, renderer);
		}
	}

	void Navigation::Navmesh::RenderTriangle(const IndexTriangle& tri, SceneRenderer& renderer) const
	{
		const Rgb8 color = Rgb8(0, 0xFF, 0);
		const uint layer = 1;

		const Vector& pos1 = GetPosAtIndex(tri.idx1);
		const Vector& pos2 = GetPosAtIndex(tri.idx2);
		const Vector& pos3 = GetPosAtIndex(tri.idx3);

		renderer.RenderLine(pos1, pos2, color, layer);
		renderer.RenderLine(pos2, pos3, color, layer);
		renderer.RenderLine(pos3, pos1, color, layer);
	}

	void Navigation::Navmesh::RenderEdges(SceneRenderer& renderer) const
	{
		for (const IndexPair& e : m_Edges)
		{
			RenderEdge(e, renderer);
		}
	}

	void Navigation::Navmesh::RenderEdge(const IndexPair& e, SceneRenderer& renderer) const
	{
		const Rgb8 color = Rgb8(0xFF, 0, 0);
		const uint layer = 1;

		const Vector& pos1 = GetPosAtIndex(e.first);
		const Vector& pos2 = GetPosAtIndex(e.second);

		renderer.RenderLine(pos1, pos2, color, layer);
	}

	void Navigation::Navmesh::RenderVertexIndices(SceneRenderer& renderer) const
	{
		const uint layer = 3;
		const uint font_size = 1;

		for (uint i = 0; i < m_Vertices.size(); i++)
		{
			const Vector& pos = m_Vertices[i];

			std::string label = std::to_string(i);
			renderer.RenderString(label.c_str(), pos, font_size, layer);
		}
	}

	void Navigation::Navmesh::RenderEdgeIndices(SceneRenderer& renderer) const
	{
		const uint layer = 3;
		const uint font_size = 5;

		for (uint i = 0; i < m_Edges.size(); i++)
		{
			const IndexPair& pos = m_Edges[i];

			const Vector edge_middle = (GetPosAtIndex(pos.first) + GetPosAtIndex(pos.second)) / 2.0f;

			std::string label = std::to_string(i);
			renderer.RenderString(label.c_str(), edge_middle, font_size, layer);
		}
	}

	void Navigation::Navmesh::RenderGraph(SceneRenderer& renderer) const
	{
		const Rgb8 color = Rgb8(0, 0, 0xFF);
		const uint layer = 1;

		const Vector vertex_size = Vector(1, 1);

		for (const graph::PositionNode& node : m_Graph.GetNodes())
		{
			renderer.RenderRect(Rect(node.GetPos() - vertex_size / 2.0f, vertex_size), color, layer);

			for (graph::NodeId neighbour_id : node.GetNeighbours())
			{
				const graph::PositionNode* neighbour = m_Graph.GetNodeById(neighbour_id);
				FE_ASSERT(neighbour != nullptr, "Didn't find neighbour node");

				renderer.RenderLine(node.GetPos(), neighbour->GetPos(), color, layer);
			}
		}
	}

	void Navigation::Navmesh::CreateGraph()
	{
		m_Graph = {};

		// Go through every triangle pair
		for (uint i = 0; i < m_Triangles.size(); i++)
		{
			NavmeshTriangle& tri1 = m_Triangles[i];

			tri1.graph_node = m_Graph.CreateNode(GetTriangleMid(tri1.vertices));
			m_GraphNodesToTriangles.emplace(tri1.graph_node, TriangleId{ i });

			for (uint j = 0; j < i; j++)
			{
				NavmeshTriangle& older_tri = m_Triangles[j]; // This triangle already has a graph node

				if (older_tri.vertices.ContainsMutualEdge(tri1.vertices))
				{
					m_Graph.AddLink2Side(tri1.graph_node, older_tri.graph_node);
				}
			}
		}
	}

	bool Navigation::Navmesh::DoesAnyTriangleContainPos(const std::vector<NavmeshTriangle>& triangles, const Vector& pos) const
	{
		// TODO: space partitioning
		for (const NavmeshTriangle& navmesh_tri : triangles)
		{
			const Triangle tri = IndexTriangleToTriangle(navmesh_tri.vertices);
			if (tri.Contains(pos))
			{
				return true;
			}
		}

		return false;
	}

	void Navigation::Navmesh::NavmeshTriangle::GetEdgesAsIndexPairs(std::vector<IndexPair>& out_edges) const
	{
		out_edges.emplace_back(vertices.idx1, vertices.idx2);
		out_edges.emplace_back(vertices.idx1, vertices.idx3);
		out_edges.emplace_back(vertices.idx2, vertices.idx3);
	}

}

