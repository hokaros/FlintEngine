#pragma once
#include <AI/BehaviorTree/Node.h>

#include "AIContext.h"


class PlayerMoveTargetSetBehavior
	: public bt::Node<AIContext>
{
protected:
	virtual bt::ENodeStatus Update(AIContext& context) override;
};