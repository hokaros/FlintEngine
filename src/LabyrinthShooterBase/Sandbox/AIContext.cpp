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

BTNodeMediator& AIContext::GetBTNodeMediator()
{
	return m_BTNodeMediator;
}

const BTNodeMediator& AIContext::GetBTNodeMediator() const
{
	return m_BTNodeMediator;
}

ConstantMover& AIContext::GetMover()
{
	return m_Mover;
}
