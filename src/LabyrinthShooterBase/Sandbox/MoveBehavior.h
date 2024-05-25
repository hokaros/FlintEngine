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

		virtual ENodeResult Run() override;

	private:
		AIContext& m_Context;
	};
}