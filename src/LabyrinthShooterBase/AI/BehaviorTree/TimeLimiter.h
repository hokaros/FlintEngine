#pragma once
#include <AI/BehaviorTree/Decorator.h>
#include "AIContext.h"

namespace bt
{
	class TimeLimiter
		: public bt::Decorator<AIContext>
	{
	public:
		TimeLimiter(float time);

		virtual void Init() override;
		virtual ENodeStatus Update(AIContext& context) override;

	private:
		float m_TimeLimit;

		float m_TimeElapsed;
	};
}