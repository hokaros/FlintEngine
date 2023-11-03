#pragma once
#include "Draw.h"
#include "Vector.h"

class DebugRenderer
{
public:
	DebugRenderer(SDL_Surface* screen);

	static void DrawLine(const Vector& start, const Vector& end, Rgb8 color);

private:
	void DrawLineImpl(const Vector& start, const Vector& end, Rgb8 color);

private:
	SDL_Surface* m_Screen;

	static DebugRenderer* s_Instance;
};

