#pragma once
#include <iostream>
extern "C" {
#include"SDL.h"
#include"SDL_main.h"
}

#include "Vector.h"

struct Rgb8
{
public:
	Uint8 r;
	Uint8 g;
	Uint8 b;

public:
	constexpr Rgb8()
		: r(0)
		, g(0)
		, b(0)
	{}

	constexpr Rgb8(Uint8 r, Uint8 g, Uint8 b)
		: r(r)
		, g(g)
		, b(b)
	{}
};

SDL_Rect RectToSDLRect(const Rect& rect);
Rect SDLRectToRect(const SDL_Rect& sdl_rect);