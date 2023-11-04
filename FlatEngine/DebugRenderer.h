#pragma once
#include "Draw.h"
#include "Vector.h"

class DebugRenderer
{
public:
	DebugRenderer(SDL_Renderer* renderer);

	// TODO: let's delete this singleton and pass the DebugRenderer directly to IDebugRenderers
	static void DrawLine(const Vector& start, const Vector& end, Rgb8 color);

private:
	void DrawLineImpl(const Vector& start, const Vector& end, Rgb8 color);

private:
	SDL_Renderer* m_Renderer;

	static DebugRenderer* s_Instance;
};

