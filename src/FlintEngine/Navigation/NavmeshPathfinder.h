#pragma once
#include <Navigation/Navmesh.h>

namespace Navigation
{
	class NavmeshPath
	{
	public:
		using iterator = std::vector<Vector>::iterator;
		using const_iterator = std::vector<Vector>::const_iterator;

		void AddControlPoint(const Vector& point);
		void Clear();

		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;

	private:
		std::vector<Vector> m_ControlPoints;
	};

	class NavmeshPathfinder
	{
	public:
		static void FindPath(const Navmesh& navmesh, const Vector& start_point, const Vector& end_point, NavmeshPath& out_path);
	};
}