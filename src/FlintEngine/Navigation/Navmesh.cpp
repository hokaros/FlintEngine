#include "Navmesh.h"

#include <SceneRenderer.h>
#include <utility.h>

Navigation::Navmesh::Navmesh()
{
	PrepareTestNavmesh();
}

void Navigation::Navmesh::Render(SceneRenderer& renderer)
{
	for (const IndexTriangle& tri : m_Triangles)
	{
		RenderTriangle(tri, renderer);
	}
}

void Navigation::Navmesh::PrepareTestNavmesh()
{
	m_Vertices.push_back(Vector(0, 0));
	m_Vertices.push_back(Vector(3, 3));
	m_Vertices.push_back(Vector(0, 3));

	m_Triangles.push_back(IndexTriangle(0, 1, 2));
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
