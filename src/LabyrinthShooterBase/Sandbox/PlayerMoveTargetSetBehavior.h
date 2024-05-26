#pragma once
#include <AI/BehaviorTree/Node.h>

#include "AIContext.h"


class PlayerMoveTargetSetBehavior
	: public bt::Node
{
public:
	PlayerMoveTargetSetBehavior(AIContext& context);

protected:
	virtual bt::ENodeStatus Update() override;

private:
	AIContext& m_Context;
};