#include "NavmeshGenerator.h"
#include <iostream>

namespace Navigation
{
	void NavmeshGenerator::Generate(const IGameObjectContainer& context, Navmesh& navmesh)
	{
		// TODO: get all colliders
		std::cout << "Generating NavMesh" << std::endl;
		PrepareTestNavmesh(navmesh);
	}

	void NavmeshGenerator::PrepareTestNavmesh(Navmesh& navmesh)
	{
		navmesh.AddVertex(Vector(0, 0));
		navmesh.AddVertex(Vector(3, 3));
		navmesh.AddVertex(Vector(0, 3));

		navmesh.AddTriangle(IndexTriangle(0, 1, 2));
	}
}