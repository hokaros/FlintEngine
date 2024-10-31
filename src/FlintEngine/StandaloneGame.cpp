#include "StandaloneGame.h"

#include <Serialization/SceneLoader.h>

StandaloneGame::StandaloneGame(const VectorInt& screen_size)
	: m_Window(screen_size.x, screen_size.y)
{
}

void StandaloneGame::Run(const char* starting_scene_path)
{
	const bool window_initialized = m_Window.Init();
	if (!window_initialized)
		return;

	GameBase game(&m_Window, &m_Window.GetSceneRenderer(), m_Input);

	game.LoadScene(SceneLoader::LoadScene(starting_scene_path));

	game.Run();
}
