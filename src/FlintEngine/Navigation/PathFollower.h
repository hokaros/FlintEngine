#pragma once
#include <Navigation/NavmeshPath.h>
#include <Navigation/IMovable.h>

namespace Navigation
{
	class PathFollower
	{
	public:
		void Update(IMovable& movable);

		void StartPath(NavmeshPath&& path);

	private:
		void RemoveCompletedControlPoints(const IMovable& movable);
		bool HasReachedControlPoint(const IMovable& movable, const Vector& controlPoint) const;

	private:
		NavmeshPath m_CurrentPath;
	};
}