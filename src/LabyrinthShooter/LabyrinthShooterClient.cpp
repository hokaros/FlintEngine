#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_main.h>
#include <LabyrinthScene.h>
#include <GameBase.h>
#include <AssetManager.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

std::unique_ptr<GameBase> CreateGame(Window& window, IInputController& input_controller)
{
	return std::make_unique<GameBase>(&window, &window.GetSceneRenderer(), input_controller);
}

int main()
{
	Window window(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!window.Init())
		return 1;

	InputController input;
	AssetManager asset_manager;

	std::unique_ptr<GameBase> game = CreateGame(window, input); // TODO: pass AssetManager

	game->LoadScene(std::make_unique<LabyrinthScene>());
	game->Run();

	return 0;
}