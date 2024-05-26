#include "MoveBehavior.h"

namespace bt
{
	MoveBehavior::MoveBehavior(AIContext& context)
		: m_Context(context)
	{
	}

	ENodeStatus MoveBehavior::Update()
	{
		m_Context.GetMover().SetDirection(Vector(Direction::EAST));
		return ENodeStatus::Success;
	}
}