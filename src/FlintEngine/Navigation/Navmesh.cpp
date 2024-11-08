#include "Navmesh.h"

#include <SceneRenderer.h>
#include <utility.h>

void Navigation::Navmesh::Render(SceneRenderer& renderer) const
{
	for (const IndexTriangle& tri : m_Triangles)
	{
		RenderTriangle(tri, renderer);
	}
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
	m_Triangles.push_back(std::move(tri));
}

void Navigation::Navmesh::AddTriangles(const std::vector<IndexTriangle>& triangles)
{
	for (const IndexTriangle& tri : triangles)
	{
		m_Triangles.push_back(tri);
	}
}

const Vector& Navigation::Navmesh::GetPosAtIndex(int idx) const
{
	FE_ASSERT(idx >= 0 && idx < m_Vertices.size(), "Index out of bounds");

	return m_Vertices[idx];
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
