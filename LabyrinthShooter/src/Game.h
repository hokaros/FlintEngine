#pragma once
#include <list>

#include <Components/RectangleRenderer.h>
#include <Components/SpriteRenderer.h>
#include <PlayerController.h>
#include <PowerBullet.h>
#include <PlayerEquipment.h>
#include <Firearm.h>
#include <Health.h>
#include "LabyrinthScene.h"

#include <GameBase.h>


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
	Game(Window* window, SceneRenderer* scene_renderer, IInputController& input_controller, GameStartInfo&& gameInfo);

protected:
	virtual std::unique_ptr<Scene> CreateScene() override;

private:
	GameStartInfo startInfo;

	AssetManager m_AssetManager;
};