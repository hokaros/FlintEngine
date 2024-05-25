#include "AIContext.h"

AIContext::AIContext(AIPerception& perception, ConstantMover& mover)
	: m_Perception(perception)
	, m_Mover(mover)
{
}

AIPerception& AIContext::GetPerception()
{
	return m_Perception;
}

ConstantMover& AIContext::GetMover()
{
	return m_Mover;
}
