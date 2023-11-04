#pragma once
#include "Draw.h"
#include "Vector.h"

SDL_Rect RectToSDLRect(const Rect& rect); // TODO: move to Draw utilities or sth
Rect SDLRectToRect(const SDL_Rect& sdl_rect);

class DebugRenderer
{
public:
	DebugRenderer(SDL_Renderer* renderer);

	// TODO: let's delete this singleton and pass the DebugRenderer directly to IDebugRenderers
	static void DrawLine(const Vector& start, const Vector& end, const Rgb8& color);
	static void DrawRect(const Rect& rect, const Rgb8& color);
	static void DrawWireRect(const Rect& rect, const Rgb8& color);

private:
	void DrawLineImpl(const Vector& start, const Vector& end, const Rgb8& color);
	void DrawRectImpl(const Rect& rect, const Rgb8& color);
	void DrawWireRectImpl(const Rect& rect, const Rgb8& color);

private:
	SDL_Renderer* m_Renderer;

	static DebugRenderer* s_Instance;
};

