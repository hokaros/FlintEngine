#pragma once
#include "AIPerception.h"
#include "BTNodeMediator.h"

#include <ConstantMover.h>

class AIContext
{
public:
	AIContext(AIPerception& perception, ConstantMover& mover);

	AIPerception& GetPerception();

	BTNodeMediator& GetBTNodeMediator();
	const BTNodeMediator& GetBTNodeMediator() const;

	ConstantMover& GetMover();

private:
	BTNodeMediator m_BTNodeMediator;

	ConstantMover& m_Mover; // Maybe hide in ActionExecutor?
	AIPerception& m_Perception;
};