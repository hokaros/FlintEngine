#include "MoveBehavior.h"

namespace bt
{
	MoveBehavior::MoveBehavior(AIContext& context)
		: m_Context(context)
	{
	}

	ENodeResult MoveBehavior::Run()
	{
		m_Context.GetMover().SetDirection(Vector(Direction::EAST));
		return ENodeResult::Success;
	}
}