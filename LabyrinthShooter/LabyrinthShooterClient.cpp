#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_main.h>
#include "Game.h"
#include "PlayerPositionsGenerator.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

std::unique_ptr<Game> CreateGame(Window& window)
{
	Vector player_start_pos = Vector(260, 160);

	GameStartInfo gameInfo = GameStartInfo(player_start_pos);
	return std::make_unique<Game>(&window, std::move(gameInfo));
}

int main()
{
	Window window(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!window.Init())
		return 1;

	InputController input;

	std::unique_ptr<Game> game = CreateGame(window);

	game->Run();

	return 0;
}