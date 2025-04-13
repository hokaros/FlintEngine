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

	void PathFollower::RemoveCompletedControlPoints(const IMovable& movable)
	{
		while (!m_CurrentPath.IsEmpty())
		{
			const Vector& currentTarget = m_CurrentPath.GetFirstControlPoint();
			if (HasReachedControlPoint(movable, currentTarget))
			{
				m_CurrentPath.RemoveFirstControlPoint();
			}
			else
			{
				break;
			}
		}
	}

	bool PathFollower::HasReachedControlPoint(const IMovable& movable, const Vector& controlPoint) const
	{
		constexpr float tolerance = 10.0f;
		const float distSq = Vector::GetDistanceSq(movable.GetPosition(), controlPoint);
		return distSq < tolerance * tolerance;
	}
}