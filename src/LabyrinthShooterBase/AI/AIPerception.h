#pragma once
#include <Core/GameObject.h>
#include <optional>

// Interface for querying data about the world
class AIPerception
{
public:
	AIPerception(const GameObject& game_object);

	const Vector& GetOwnPosition() const;
	std::optional<Vector> GetPlayerPosition() const;

	double GetDeltaTime() const;


private:
	const GameObject& m_GameObject;

	const IGameObject* m_Player = nullptr;
};