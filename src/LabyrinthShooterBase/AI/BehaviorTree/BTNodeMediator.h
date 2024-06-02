#pragma once
#include <Vector.h>

class BTNodeMediator
{
public:
	void SetMoveTarget(const Vector& target_pos);
	const Vector& GetMoveTarget() const;

private:
	Vector m_MoveTarget;
};