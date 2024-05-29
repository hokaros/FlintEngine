#pragma once
#include <AI/BehaviorTree/Node.h>

#include "AIContext.h"

class SetRandomMoveTargetBehavior
	: public bt::Node
{
public:
	SetRandomMoveTargetBehavior(AIContext& context);

protected:
	virtual bt::ENodeStatus Update() override;

private:
	const Vector& ChooseOffset();

private:
	AIContext& m_Context;

	std::vector<Vector> m_Offsets;
	size_t m_LastTarget;
};