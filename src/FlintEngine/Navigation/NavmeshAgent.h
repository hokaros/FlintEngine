#pragma once
#include <Math/Vector.h>
#include "Navmesh.h"

namespace Navigation
{
	class NavmeshAgent
	{
	public:
		NavmeshAgent(const Navmesh& navmesh);

		void SetTarget(const Vector& pos);

	private:
		const Navmesh& m_Navmesh;
		// TODO: path follower
	};
}