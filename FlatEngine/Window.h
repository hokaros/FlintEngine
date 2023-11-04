#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_main.h>
#include <list>
#include "DebugRenderer.h"

struct TextureRenderArgs {
	SDL_Texture* texture;
	SDL_Rect rect;
	double angle;

	TextureRenderArgs(SDL_Texture* texture, SDL_Rect rect, double angle)
		: texture(texture), rect(rect), angle(angle) {}
};

// TODO: IDebugRenderer

// TODO: FRenderer jako API do renderowania

class Window
{
public:
	Window(int width, int height);
	~Window();

	bool Init();
	void Render();

	void RenderTexture(SDL_Texture* texture, const SDL_Rect& rect, double angle);

	// narysowanie napisu txt na ekranie, zaczynaj¹c od punktu (x, y)
	void DrawString(int x, int y, const char* text, int fontSize);

	SDL_Renderer* GetRenderer() const;

	int GetWidth() const;
	int GetHeight() const;

	static Window* Main();

private:
	bool LoadCharsets();

private:
	int m_Width;
	int m_Height;

	SDL_Surface* m_Charset = nullptr; // TODO: remove
	SDL_Surface* m_BigCharset = nullptr;
	SDL_Texture* m_CharsetTex = nullptr;
	SDL_Texture* m_Scrtex = nullptr;
	SDL_Window* m_Window = nullptr;
	SDL_Renderer* m_Renderer = nullptr;

	DebugRenderer* m_DebugRenderer = nullptr;

	static Window* s_MainWindow;
};

