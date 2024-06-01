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


class Game
	: public GameBase
{
public:
	Game(Window* window, SceneRenderer* scene_renderer, IInputController& input_controller);

protected:
	virtual std::unique_ptr<Scene> CreateScene() override;

private:
	AssetManager m_AssetManager;
};