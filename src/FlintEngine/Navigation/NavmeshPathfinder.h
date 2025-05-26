#pragma once
#include <Navigation/Navmesh.h>
#include <Navigation/NavmeshPath.h>

namespace Navigation
{
	class NavmeshPathfinder
	{
	public:
		static void FindPath(const Navmesh& navmesh, const Vector& start_point, const Vector& end_point, float min_width, NavmeshPath& out_path);

	private:
		static void FindTriangleCenterPath(const Navmesh& navmesh, const Vector& start_point, const Vector& end_point, NavmeshPath& out_path);
		static void CollapsePathControlPointsWhileClearSight(NavmeshPath& path, const Navmesh& navmesh, float min_width);
		static void PullPathControlPointsWhileClearSight(NavmeshPath& path, const Navmesh& navmesh, float min_width);

		static constexpr size_t s_MaxPullingIterationsPerSegment = 8;
	};
}