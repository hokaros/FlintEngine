#include "AIPerception.h"

#include <Scene/Scene.h>
#include <Shared.h>

AIPerception::AIPerception(const GameObject& game_object)
	: m_GameObject(game_object)
{
	m_Player = m_GameObject.GetScene()->FindGameObjectByName(PLAYER_GO_NAME);
}

const Vector& AIPerception::GetOwnPosition() const
{
	return m_GameObject.GetWorldPosition();
}

std::optional<Vector> AIPerception::GetPlayerPosition() const
{
	if (m_Player == nullptr)
		return std::nullopt;

	// TODO: AI shouldn't always know where the player is
	return m_Player->GetTransformable().GetWorldPosition();
}

double AIPerception::GetDeltaTime() const
{
	return Timer::Main()->GetDeltaTime();
}
