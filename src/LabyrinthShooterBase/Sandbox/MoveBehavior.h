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
		AIContext& m_Context;
	};
}