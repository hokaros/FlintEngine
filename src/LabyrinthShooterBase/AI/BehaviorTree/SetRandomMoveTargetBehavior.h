#pragma once
#include <AI/BehaviorTree/Node.h>

#include "AIContext.h"

class SetRandomMoveTargetBehavior
	: public bt::Node<AIContext>
{
public:
	SetRandomMoveTargetBehavior();

protected:
	virtual bt::ENodeStatus Update(AIContext& context) override;

private:
	const Vector& ChooseOffset();

private:
	std::vector<Vector> m_Offsets;
	size_t m_LastTarget;
};