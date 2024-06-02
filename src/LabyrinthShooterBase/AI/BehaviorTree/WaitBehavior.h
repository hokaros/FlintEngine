#pragma once
#include <AI/BehaviorTree/Node.h>
#include "AIContext.h"

class WaitBehavior
	: public bt::Node<AIContext>
{
public:
	WaitBehavior(float time);

protected:
	virtual void Init() override;
	virtual bt::ENodeStatus Update(AIContext& context) override;

private:
	float m_TotalTime = 0.f;
	float m_TimeLeft = 0.f;
};