#include "DebugRenderer.h"

DebugRenderer* DebugRenderer::s_Instance = nullptr;

DebugRenderer::DebugRenderer(SDL_Surface* screen)
	: m_Screen(screen)
{
	s_Instance = this;
}

void DebugRenderer::DrawLine(const Vector& start, const Vector& end, Rgb8 color)
{
	s_Instance->DrawLineImpl(start, end, color);
}

void DebugRenderer::DrawLineImpl(const Vector& start, const Vector& end, Rgb8 color)
{
	Uint32 color32 = SDL_MapRGB(m_Screen->format, color.r, color.g, color.b);
	draw::DrawLine(m_Screen, start.x, start.y, 10, 1, 0, color32);
}
