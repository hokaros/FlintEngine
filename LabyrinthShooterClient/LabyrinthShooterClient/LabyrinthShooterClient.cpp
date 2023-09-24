#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_main.h>
#include <Game.h>
#include <PlayerPositionsGenerator.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

Game* CreateGame(Window& window)
{
	PlayerPositionsGenerator positions_generator;
	positions_generator.Generate(PLAYERS_NUM, WIDTH, HEIGHT, MAP_START_X, MAP_START_Y);

	std::vector<Vector> positions = positions_generator.GetPlayerPositions();
	Vector* positions_arr = new Vector[positions.size()];
	for (size_t i = 0; i < positions.size(); i++)
	{
		positions_arr[i] = positions[i];
	}

	GameStartInfo gameInfo = GameStartInfo(positions_arr, 1, 0);
	return new Game(&window, std::move(gameInfo));
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

	Game* game = CreateGame(window);

	game->Run();

	delete game;
	return 0;
}