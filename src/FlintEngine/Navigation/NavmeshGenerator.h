#pragma once
#include <Core/IGameObjectContainer.h>
#include "Navmesh.h"

class Scene;

namespace Navigation
{
	class NavmeshGenerator
	{
	public:
		static void Generate(const IGameObjectContainer& context, Navmesh& navmesh);
	};
}