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

TextureBlendModeScope::TextureBlendModeScope(SDL_Texture* texture, SDL_BlendMode blend_mode)
	: m_Texture(texture)
{
	SDL_GetTextureBlendMode(m_Texture, &m_PrevBlendMode);
	SDL_SetTextureBlendMode(m_Texture, blend_mode);
}

TextureBlendModeScope::~TextureBlendModeScope()
{
	SDL_SetTextureBlendMode(m_Texture, m_PrevBlendMode);
}
