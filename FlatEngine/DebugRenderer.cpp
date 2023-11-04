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

void DebugRenderer::DrawRect(const Vector& min, const Vector& size, const Rgb8& color)
{
	s_Instance->DrawRectImpl(min, size, color);
}

void DebugRenderer::DrawWireRect(const Vector& min, const Vector& size, const Rgb8& color)
{
	s_Instance->DrawWireRectImpl(min, size, color);
}

void DebugRenderer::DrawLineImpl(const Vector& start, const Vector& end, const Rgb8& color)
{
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, 0xFF);

	int result = SDL_RenderDrawLine(m_Renderer, start.x, start.y, end.x, end.y);
	FE_ASSERT(result == 0, "ERROR: Could not render");
}

void DebugRenderer::DrawRectImpl(const Vector& min, const Vector& size, const Rgb8& color)
{
	SDL_Rect rect;
	rect.x = min.x;
	rect.y = min.y;
	rect.w = size.x;
	rect.h = size.y;

	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, 0xFF);

	int result = SDL_RenderFillRect(m_Renderer, &rect);
	FE_ASSERT(result == 0, "ERROR: Could not render");
}

void DebugRenderer::DrawWireRectImpl(const Vector& min, const Vector& size, const Rgb8& color)
{
	// TODO: receive Rect and convert with RectToSDLRect()
	SDL_Rect rect;
	rect.x = min.x;
	rect.y = min.y;
	rect.w = size.x;
	rect.h = size.y;

	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, 0xFF);

	int result = SDL_RenderDrawRect(m_Renderer, &rect);
	FE_ASSERT(result == 0, "ERROR: Could not render");
}

SDL_Rect DebugRenderer::RectToSDLRect(const Rect& rect)
{
	SDL_Rect sdl_rect;
	sdl_rect.x = rect.pos.x;
	sdl_rect.y = rect.pos.y;
	sdl_rect.w = rect.size.x;
	sdl_rect.h = rect.size.y;
	return sdl_rect;
}
