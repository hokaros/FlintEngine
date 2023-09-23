#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_main.h>
#include <list>

struct TextureRenderArgs {
	SDL_Texture* texture;
	SDL_Rect rect;
	double angle;

	TextureRenderArgs(SDL_Texture* texture, SDL_Rect rect, double angle)
		: texture(texture), rect(rect), angle(angle) {}
};

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

	SDL_Surface* GetScreen() const;
	SDL_Renderer* GetRenderer() const;

	int GetWidth() const;
	int GetHeight() const;

	static Window* Main();
private:
	int width;
	int height;

	SDL_Surface* screen = NULL;
	SDL_Surface* charset = NULL;
	SDL_Surface* bigCharset = NULL;
	SDL_Texture* scrtex = NULL;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	std::list<TextureRenderArgs> renderTextures;

	static Window* mainWindow;

private:
	bool LoadCharsets();
};

