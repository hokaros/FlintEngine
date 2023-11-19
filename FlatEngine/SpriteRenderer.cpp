#include "SpriteRenderer.h"
#include "Draw.h"
#include "GameObject.h"

SpriteRenderer::SpriteRenderer(SDL_Surface* sprite)
	: m_Sprite(sprite)
	, m_Texture(SDL_CreateTextureFromSurface(Window::Main()->GetRenderer(), sprite)) {

}

SpriteRenderer::~SpriteRenderer() {
	SDL_DestroyTexture(m_Texture);
}

void SpriteRenderer::Render() {
	SDL_Renderer* renderer = Window::Main()->GetRenderer();

	SDL_Rect dstRect;
	dstRect.x = m_GameObject->GetPosition().x;
	dstRect.y = m_GameObject->GetPosition().y;
	dstRect.w = m_GameObject->GetSize().x;
	dstRect.h = m_GameObject->GetSize().y;

	Window::Main()->RenderTexture(m_Texture, dstRect, m_GameObject->GetRotation());
}

std::unique_ptr<ObjectComponent> SpriteRenderer::Copy() {
	return std::make_unique<SpriteRenderer>(m_Sprite);
}