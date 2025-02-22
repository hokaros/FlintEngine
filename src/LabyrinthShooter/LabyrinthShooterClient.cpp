#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_main.h>
#include <StandaloneGame.h>

int main()
{
	constexpr VectorInt window_size(800, 600);
	constexpr const char* starting_scene_path = "Assets/main.scene";

	StandaloneGame game(window_size);
	//game.GetWindow().GetSceneRenderer().SetViewport(Rect(Vector(100, -300), Vector(700, 700)));

	game.Run(starting_scene_path);

	return 0;
}