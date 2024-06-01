#pragma once
#include <SDL.h>
#include <SDL_main.h>

class RenderTargetScope
{
public:
	RenderTargetScope(SDL_Renderer* renderer, SDL_Texture* new_render_target);
	~RenderTargetScope();

private:
	SDL_Texture* m_PrevRenderTarget = nullptr;
	SDL_Renderer* m_Renderer = nullptr;
};