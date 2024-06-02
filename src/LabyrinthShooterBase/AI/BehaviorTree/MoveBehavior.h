#pragma once
#include <AI/BehaviorTree/Node.h>

#include "AIContext.h"

namespace bt
{
	class MoveBehavior
		: public Node<AIContext>
	{
	protected:
		virtual ENodeStatus Update(AIContext& context) override;

	private:
		const Vector& GetTargetPosition(AIContext& context) const;
		const Vector& GetOwnPosition(AIContext& context) const;
		bool IsPositionWithinTolerance(const Vector& position, AIContext& context) const;

	private:
		static constexpr float TARGET_TOLERANCE = 1.0f;
		static constexpr float TARGET_TOLERANCE_SQ = TARGET_TOLERANCE * TARGET_TOLERANCE;
	};
}