#pragma once
#include <Math/Vector.h>
#include <Navigation/Navmesh.h>
#include <Navigation/PathFollower.h>

namespace Navigation
{
	class NavmeshAgent
	{
	public:
		NavmeshAgent(const Navmesh& navmesh, IMovable& movable, float width);

		void Update();

		void SetTarget(const Vector& targetPos);

	private:
		const Navmesh& m_Navmesh;
		IMovable& m_Movable;

		PathFollower m_PathFollower;
		float m_Width;
	};
}