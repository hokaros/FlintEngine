#include "MoveBehavior.h"

namespace bt
{
	ENodeStatus MoveBehavior::Update(AIContext& context)
	{
		const Vector& target_pos = GetTargetPosition(context);

		if (IsPositionWithinTolerance(target_pos, context))
		{
			context.GetMover().SetDirection(Vector::ZERO);
			return ENodeStatus::Success;
		}

		Vector to_target = target_pos - GetOwnPosition(context);
		context.GetMover().SetDirection(to_target);

		return ENodeStatus::InProgress;
	}

	const Vector& MoveBehavior::GetTargetPosition(AIContext& context) const
	{
		return context.GetBTNodeMediator().GetMoveTarget();
	}

	const Vector& MoveBehavior::GetOwnPosition(AIContext& context) const
	{
		return context.GetPerception().GetOwnPosition();
	}

	bool MoveBehavior::IsPositionWithinTolerance(const Vector& position, AIContext& context) const
	{
		return (GetOwnPosition(context) - position).LengthSquared() <= TARGET_TOLERANCE_SQ;
	}
}