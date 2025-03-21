#include "Navmesh.h"

#include <SceneRenderer.h>
#include <utility.h>

#include <string>

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
		renderer.RenderRect(Rect(node.GetPos() - vertex_size /2.0f, vertex_size), color, layer);

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
