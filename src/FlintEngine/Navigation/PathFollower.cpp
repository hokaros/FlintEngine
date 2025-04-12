#include "PathFollower.h"

namespace Navigation
{
	void PathFollower::Update(IMovable& movable)
	{
		RemoveCompletedControlPoints(movable);

		if (m_CurrentPath.IsEmpty())
		{
			movable.Stop();
		}
		else
		{
			const Vector& currentTarget = m_CurrentPath.GetFirstControlPoint();
			movable.MoveTowards(currentTarget);
		}
	}

	void PathFollower::StartPath(NavmeshPath&& path)
	{
		m_CurrentPath = std::move(path);
	}
}