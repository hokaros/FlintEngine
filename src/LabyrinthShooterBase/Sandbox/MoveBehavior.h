#pragma once
#include <AI/BehaviorTree/Node.h>

#include "AIContext.h"

namespace bt
{
	class MoveBehavior
		: public Node
	{
	public:
		MoveBehavior(AIContext& context);

	protected:
		virtual ENodeStatus Update() override;

	private:
		const Vector& GetTargetPosition() const;
		const Vector& GetOwnPosition() const;
		bool IsPositionWithinTolerance(const Vector& position) const;

	private:
		AIContext& m_Context;

		static constexpr float TARGET_TOLERANCE = 50.0f;
		static constexpr float TARGET_TOLERANCE_SQ = TARGET_TOLERANCE * TARGET_TOLERANCE;
	};
}