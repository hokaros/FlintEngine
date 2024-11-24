#include "Navmesh.h"

#include <SceneRenderer.h>
#include <utility.h>

void Navigation::Navmesh::Render(SceneRenderer& renderer) const
{
	RenderTriangles(renderer);
	//RenderEdges(renderer);
	RenderGraph(renderer);
}

void Navigation::Navmesh::Clear()
{
	m_Vertices.clear();
	m_Triangles.clear();
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

Vector Navigation::Navmesh::GetTriangleMid(const IndexTriangle& tri) const
{
	const Vector& v1 = GetPosAtIndex(tri.idx1);
	const Vector& v2 = GetPosAtIndex(tri.idx2);
	const Vector& v3 = GetPosAtIndex(tri.idx3);

	return (v1 + v2 + v3) / 3.0f;
}

void Navigation::Navmesh::RenderTriangles(SceneRenderer& renderer) const
{
	for (const Triangle& tri : m_Triangles)
	{
		RenderTriangle(tri.vertices, renderer);
	}
}

void Navigation::Navmesh::RenderTriangle(const IndexTriangle& tri, SceneRenderer& renderer) const
{
	const Rgb8 color = Rgb8(0xFF, 0, 0);
	const uint layer = 1;

	const Vector& pos1 = GetPosAtIndex(tri.idx1);
	const Vector& pos2 = GetPosAtIndex(tri.idx2);
	const Vector& pos3 = GetPosAtIndex(tri.idx3);

	renderer.RenderLine(pos1, pos2, color, layer);
	renderer.RenderLine(pos2, pos3, color, layer);
	renderer.RenderLine(pos3, pos1, color, layer);

	// TODO: filling the triangle
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
		Triangle& tri1 = m_Triangles[i];

		tri1.graph_node = m_Graph.CreateNode(GetTriangleMid(tri1.vertices));

		for (uint j = 0; j < i; j++)
		{
			Triangle& older_tri = m_Triangles[j]; // This triangle already has a graph node

			if (older_tri.vertices.ContainsMutualEdge(tri1.vertices))
			{
				m_Graph.AddLink2Side(tri1.graph_node, older_tri.graph_node);
			}
		}
	}
}
