#include "DebugRenderer.h"
#include "Window.h"
#include "utility.h"

DebugRenderer* DebugRenderer::s_Instance = nullptr;

DebugRenderer::DebugRenderer(SDL_Renderer* renderer)
	: m_Renderer(renderer)
{
	s_Instance = this;
}

void DebugRenderer::DrawLine(const Vector& start, const Vector& end, const Rgb8& color)
{
	s_Instance->DrawLineImpl(start, end, color);
}

void DebugRenderer::DrawRect(const Rect& rect, const Rgb8& color)
{
	s_Instance->DrawRectImpl(rect, color);
}

void DebugRenderer::DrawWireRect(const Rect& rect, const Rgb8& color)
{
	s_Instance->DrawWireRectImpl(rect, color);
}

void DebugRenderer::DrawLineImpl(const Vector& start, const Vector& end, const Rgb8& color)
{
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, 0xFF);

	int result = SDL_RenderDrawLine(m_Renderer, start.x, start.y, end.x, end.y);
	FE_ASSERT(result == 0, "ERROR: Could not render");
}

void DebugRenderer::DrawRectImpl(const Rect& rect, const Rgb8& color)
{
	SDL_Rect sdl_rect = RectToSDLRect(rect);

	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, 0xFF);

	int result = SDL_RenderFillRect(m_Renderer, &sdl_rect);
	FE_ASSERT(result == 0, "ERROR: Could not render");
}

void DebugRenderer::DrawWireRectImpl(const Rect& rect, const Rgb8& color)
{
	SDL_Rect sdl_rect = RectToSDLRect(rect);

	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, 0xFF);

	int result = SDL_RenderDrawRect(m_Renderer, &sdl_rect);
	FE_ASSERT(result == 0, "ERROR: Could not render");
}
