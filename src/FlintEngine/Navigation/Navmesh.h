#pragma once
#include <vector>
#include <Math/Vector.h>
#include <Math/GeometryStructures.h>

class SceneRenderer;

namespace Navigation
{
	class Navmesh
	{
	public:
		void Render(SceneRenderer& renderer) const;

		void Clear();
		void AddVertex(Vector&& v);
		void AddVertices(const std::vector<Vector>& vertices);
		void AddTriangle(IndexTriangle&& tri);
		void AddTriangles(const std::vector<IndexTriangle>& triangles);

	private:
		const Vector& GetPosAtIndex(int idx) const;

		void RenderTriangle(const IndexTriangle& tri, SceneRenderer& renderer) const;

	private:
		std::vector<Vector> m_Vertices;
		std::vector<IndexTriangle> m_Triangles;
	};
}