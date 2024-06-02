#include "WaitBehavior.h"

WaitBehavior::WaitBehavior(float time)
	: m_TotalTime(time)
{
}

void WaitBehavior::Init()
{
	m_TimeLeft = m_TotalTime;
}

bt::ENodeStatus WaitBehavior::Update(AIContext& context)
{
	if (m_TimeLeft <= 0.f)
		return bt::ENodeStatus::Success;

	m_TimeLeft -= context.GetPerception().GetDeltaTime();

	return bt::ENodeStatus::InProgress;
}
