#pragma once
#include <AI/BehaviorTree/Node.h>
#include "AIContext.h"

class WaitBehavior
	: public bt::Node
{
public:
	WaitBehavior(float time, AIContext& context);

protected:
	virtual void Init() override;
	virtual bt::ENodeStatus Update() override;

private:
	AIContext& m_Context;

	float m_TotalTime = 0.f;
	float m_TimeLeft = 0.f;
};