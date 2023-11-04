#include "DebugRenderer.h"
#include "Window.h"

DebugRenderer* DebugRenderer::s_Instance = nullptr;

DebugRenderer::DebugRenderer(SDL_Renderer* renderer)
	: m_Renderer(renderer)
{
	s_Instance = this;
}

void DebugRenderer::DrawLine(const Vector& start, const Vector& end, Rgb8 color)
{
	s_Instance->DrawLineImpl(start, end, color);
}

void DebugRenderer::DrawLineImpl(const Vector& start, const Vector& end, Rgb8 color)
{
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, 0xFF);
	int result = SDL_RenderDrawLine(m_Renderer, start.x, start.y, end.x, end.y);
	if (result != 0)
	{
		std::cout << "ERROR: Could not render" << std::endl;
	}
}
