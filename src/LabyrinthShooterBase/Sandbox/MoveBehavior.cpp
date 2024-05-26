#include "MoveBehavior.h"

namespace bt
{
	MoveBehavior::MoveBehavior(AIContext& context)
		: m_Context(context)
	{
	}

	ENodeStatus MoveBehavior::Update()
	{
		const Vector& target_pos = GetTargetPosition();

		if (IsPositionWithinTolerance(target_pos))
		{
			m_Context.GetMover().SetDirection(Vector::ZERO);
			return ENodeStatus::Success;
		}

		Vector to_target = target_pos - GetOwnPosition();
		m_Context.GetMover().SetDirection(to_target);

		return ENodeStatus::Success;
	}

	const Vector& MoveBehavior::GetTargetPosition() const
	{
		return m_Context.GetBTNodeMediator().GetMoveTarget();
	}

	const Vector& MoveBehavior::GetOwnPosition() const
	{
		return m_Context.GetPerception().GetOwnPosition();
	}

	bool MoveBehavior::IsPositionWithinTolerance(const Vector& position) const
	{
		return (GetOwnPosition() - position).LengthSquared() <= TARGET_TOLERANCE_SQ;
	}
}