#include "BTNodeMediator.h"

void BTNodeMediator::SetMoveTarget(const Vector& target_pos)
{
	m_MoveTarget = target_pos;
}

const Vector& BTNodeMediator::GetMoveTarget() const
{
	return m_MoveTarget;
}
