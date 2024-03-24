#include "Game.h"

Game::Game(Window* window, IInputController& input_controller, GameStartInfo&& gameInfo)
	: GameBase(window, input_controller)
	, startInfo(std::move(gameInfo)) 
{
}

std::unique_ptr<Scene> Game::CreateScene()
{
	return std::make_unique<LabyrinthScene>(startInfo.GetPlayerPosition(), m_AssetManager);
}




GameStartInfo::GameStartInfo(const Vector& player_position)
	: m_PlayerPosition(player_position) 
{

}

const Vector& GameStartInfo::GetPlayerPosition() const {
	return m_PlayerPosition;
}