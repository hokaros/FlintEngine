#include "Game.h"

Game::Game(Window* window, SceneRenderer* scene_renderer, IInputController& input_controller)
	: GameBase(window, scene_renderer, input_controller)
{
}

std::unique_ptr<Scene> Game::CreateScene()
{
	return std::make_unique<LabyrinthScene>(m_AssetManager);
}