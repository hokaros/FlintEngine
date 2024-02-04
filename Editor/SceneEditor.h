#pragma once
#include "../FlatEngine/SceneRenderer.h"
#include "../FlatEngine/imgui/imgui.h"
#include "Scene.h"

class SceneEditor
{
public:
	SceneEditor(SDL_Renderer& renderer, float screenWidth, float screenHeight);

	void Render();

private:
	void AddExampleObjectsToScene();

private:
	SceneRenderer m_SceneRenderer;
	Scene m_Scene; // TODO: transform into Asset Handle
};

