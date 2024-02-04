#pragma once
#include "../FlatEngine/GameObject.h"
#include "../FlatEngine/SceneRenderer.h"

// TODO: move to the engine
class Scene
{
public:
	void Render();
	
	void AddGameObject(std::unique_ptr<GameObject> game_object);
	void SetBackgroundColor(const Rgb8& color);

private:
	void RenderBackground();

private:
	std::vector<std::unique_ptr<GameObject>> m_GameObjects;
	Rgb8 m_BackgroundColor = Rgb8(255, 255, 255);
};

