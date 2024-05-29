#include "WaitBehavior.h"

WaitBehavior::WaitBehavior(float time, AIContext& context)
	: m_TotalTime(time)
	, m_Context(context)
{
}

void WaitBehavior::Init()
{
	m_TimeLeft = m_TotalTime;
}

bt::ENodeStatus WaitBehavior::Update()
{
	if (m_TimeLeft <= 0.f)
		return bt::ENodeStatus::Success;

	m_TimeLeft -= m_Context.GetPerception().GetDeltaTime();

	return bt::ENodeStatus::InProgress;
}
