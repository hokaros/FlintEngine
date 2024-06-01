#include "RenderingHelpers.h"

RenderTargetScope::RenderTargetScope(SDL_Renderer* renderer, SDL_Texture* new_render_target)
	: m_Renderer(renderer)
{
	m_PrevRenderTarget = SDL_GetRenderTarget(m_Renderer);
	SDL_SetRenderTarget(m_Renderer, new_render_target);
}

RenderTargetScope::~RenderTargetScope()
{
	SDL_SetRenderTarget(m_Renderer, m_PrevRenderTarget);
}