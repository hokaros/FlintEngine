#pragma once
#include <Core/GameBase.h>
#include <AssetManager.h>

class StandaloneGame
{
public:
	StandaloneGame(const VectorInt& screen_size);

	void Run(const char* starting_scene_path);

private:
	InputController m_Input;
	AssetManager m_AssetManager;
	Window m_Window;
};