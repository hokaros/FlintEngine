#include "AIPerception.h"

AIPerception::AIPerception(const GameObject& game_object)
	: m_GameObject(game_object)
{
}

const Vector& AIPerception::GetOwnPosition() const
{
	return m_GameObject.GetPosition();
}
