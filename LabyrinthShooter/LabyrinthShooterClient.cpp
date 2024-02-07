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

// Należy tu dodać wszystkie klawisze, które chce się odczytywać podczas gry i menu
// TODO: dodać wszystkie do obsługiwanych
SDL_Keycode steeringKeys[] = { SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_a, SDLK_w, SDLK_d, SDLK_s,
	SHOOT_KEY,
	WPN_SWITCH_KEY,
	SDLK_ESCAPE,
	SDLK_RETURN,
	SDLK_0,
	SDLK_1,
	SDLK_2,
	SDLK_3,
	SDLK_4,
	SDLK_5,
	SDLK_6,
	SDLK_7,
	SDLK_8,
	SDLK_9,
	SDLK_BACKSPACE,
	(SDL_Keycode)'.'
};

int main()
{
	Window window(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!window.Init())
		return 1;

	InputController input = InputController(steeringKeys, sizeof(steeringKeys) / sizeof(SDL_Keycode));

	std::unique_ptr<Game> game = CreateGame(window);

	game->Run();

	return 0;
}