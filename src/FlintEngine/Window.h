#pragma once
#include <iostream>
#include <list>
#include "SceneRenderer.h"
#include "DebugRenderer.h"

#include "imgui/imgui.h"

// TODO: IDebugRenderer

class Window
{
public:
	Window(int width, int height);
	~Window();

	bool Init();
	void Present();

	SDL_Renderer* GetRenderer() const;
	SceneRenderer& GetSceneRenderer();

	int GetWidth() const;
	int GetHeight() const;

private:
	void InitImGui();
	void DeinitImGui();
	void ImGuiNewFrame();
	void RenderImGui();

private:
	int m_Width;
	int m_Height;

	SDL_Window* m_Window = nullptr;
	SDL_Renderer* m_Renderer = nullptr;

	SceneRenderer m_SceneRenderer;
};

