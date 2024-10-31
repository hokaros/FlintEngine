#include "NavmeshGenerator.h"

#include <iostream>
#include <Components/WalkableSurface.h>
#include <Components/BoxCollider.h>
#include <Core/GameObject.h>

namespace Navigation
{
	void NavmeshGenerator::Generate(const IGameObjectContainer& context, Navmesh& navmesh)
	{
		const std::vector<std::unique_ptr<GameObject>>& game_objects = context.GetGameObjects();

		std::vector<WalkableSurface*> walkables;
		GameObject::FindComponentsInHierarchies(game_objects, walkables);

		std::vector<BoxCollider*> colliders;
		GameObject::FindComponentsInHierarchies(game_objects, colliders);

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