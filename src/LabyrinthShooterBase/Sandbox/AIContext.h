#pragma once
#include "AIPerception.h"

#include <ConstantMover.h>

class AIContext
{
public:
	AIContext(AIPerception& perception, ConstantMover& mover);

	AIPerception& GetPerception();

	ConstantMover& GetMover();

private:
	ConstantMover& m_Mover; // Maybe hide in ActionExecutor?
	AIPerception& m_Perception;
};