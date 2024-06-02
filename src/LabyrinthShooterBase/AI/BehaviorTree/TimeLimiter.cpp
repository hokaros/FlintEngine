#include "TimeLimiter.h"

namespace bt
{
	TimeLimiter::TimeLimiter(float time)
		: m_TimeLimit(time)
		, m_TimeElapsed(0.f)
	{
	}

	void TimeLimiter::Init()
	{
		m_TimeElapsed = 0.f;
	}

	ENodeStatus TimeLimiter::Update(AIContext& context)
	{
		ChildT* child = GetChild();
		if (child == nullptr)
			return ENodeStatus::Failure;

		ENodeStatus status = child->Run(context);
		if (status != ENodeStatus::InProgress)
			return status;

		// Only InProgress state is time-limited
		m_TimeElapsed += context.GetPerception().GetDeltaTime();
		if (m_TimeElapsed >= m_TimeLimit)
			return ENodeStatus::Failure;

		return status;
	}
}
