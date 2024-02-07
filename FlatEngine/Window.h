#pragma once
#include <iostream>
#include <list>
#include "SceneRenderer.h"
#include "DebugRenderer.h"

#include "imgui/imgui.h"

// TODO: IDebugRenderer

// TODO: FRenderer jako API do renderowania

class Window
{
public:
	Window(int width, int height);
	~Window();

	bool Init();
	void Present();

	void RenderTexture(SDL_Texture* texture, const SDL_Rect& rect, double angle);

	SDL_Renderer* GetRenderer() const;

	int GetWidth() const;
	int GetHeight() const;

	static Window* Main();

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

	std::unique_ptr<DebugRenderer> m_DebugRenderer = nullptr;

	static Window* s_MainWindow;
};

