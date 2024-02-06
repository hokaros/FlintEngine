#pragma once
#include "GameObject.h"
#include "SceneRenderer.h"

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

