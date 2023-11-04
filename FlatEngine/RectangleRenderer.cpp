#include "RectangleRenderer.h"
#include "DebugRenderer.h"

RectangleRenderer::RectangleRenderer(GameObject& owner, SDL_Surface* screen, const Rgb8& color)
	:ObjectRenderer(owner, screen)
	, m_Color(color)
{

}

void RectangleRenderer::Render(SDL_Surface* screen) 
{
	DebugRenderer::DrawRect(gameObject.GetPosition(), gameObject.GetSize(), m_Color);
}

ObjectComponent* RectangleRenderer::Copy(GameObject& newOwner) 
{
	return new RectangleRenderer(newOwner, screen, m_Color);
}