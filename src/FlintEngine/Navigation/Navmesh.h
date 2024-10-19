#pragma once
#include <vector>
#include <Math/Vector.h>

class SceneRenderer;

namespace Navigation
{
	class Navmesh
	{
	public:
		Navmesh();

		void Render(SceneRenderer& renderer);

	private:
		void PrepareTestNavmesh();

	private:
		struct IndexTriangle
		{
		public:
			int idx1 = -1;
			int idx2 = -1;
			int idx3 = -1;

		public:
			constexpr IndexTriangle(int idx1, int idx2, int idx3)
				: idx1(idx1)
				, idx2(idx2)
				, idx3(idx3)
			{}
		};

	private:
		const Vector& GetPosAtIndex(int idx) const;

		void RenderTriangle(const IndexTriangle& tri, SceneRenderer& renderer) const;

	private:
		std::vector<Vector> m_Vertices;
		std::vector<IndexTriangle> m_Triangles;
	};
}