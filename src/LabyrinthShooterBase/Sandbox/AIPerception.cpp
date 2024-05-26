#include "AIPerception.h"

#include <Scene.h>
#include <Shared.h>

AIPerception::AIPerception(const GameObject& game_object)
	: m_GameObject(game_object)
{
	m_Player = m_GameObject.GetScene()->FindGameObjectByName(PLAYER_GO_NAME);
}

const Vector& AIPerception::GetOwnPosition() const
{
	return m_GameObject.GetPosition();
}

std::optional<Vector> AIPerception::GetPlayerPosition() const
{
	if (m_Player == nullptr)
		return std::nullopt;

	// TODO: AI shouldn't always know where the player is
	return m_Player->GetPosition();
}
