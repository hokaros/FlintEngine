#pragma once
#include <list>

#include "../FlatEngine/RectangleRenderer.h"
#include "../FlatEngine/SpriteRenderer.h"
#include "../LabyrinthShooterBase/PlayerController.h"
#include "../LabyrinthShooterBase/PowerBullet.h"
#include "../LabyrinthShooterBase/PlayerEquipment.h"
#include "../LabyrinthShooterBase/Firearm.h"
#include "../LabyrinthShooterBase/Health.h"
#include "LabyrinthScene.h"

#include "../FlatEngine/GameBase.h"


struct GameStartInfo {
public:
	GameStartInfo(const Vector& player_position);

	const Vector& GetPlayerPosition() const;

private:
	Vector m_PlayerPosition;
};


class Game
	: public GameBase
{
public:
	Game(Window* window, GameStartInfo&& gameInfo);

protected:
	virtual std::unique_ptr<Scene> CreateScene() override;

private:
	GameStartInfo startInfo;

	AssetManager m_AssetManager;
};