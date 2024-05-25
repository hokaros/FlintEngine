#pragma once
#include <Core/GameObject.h>

// Interface for querying data about the world
class AIPerception
{
public:
	AIPerception(const GameObject& game_object);

	const Vector& GetOwnPosition() const;

private:
	const GameObject& m_GameObject;
};