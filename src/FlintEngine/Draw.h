#pragma once
#include <iostream>

extern "C" {
#include"SDL.h"
#include"SDL_main.h"
}

#include <Math/Vector.h>
#include <Math/Rect.h>

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

	static constexpr Rgb8 FromFloat(float r, float g, float b)
	{
		return Rgb8(r * 0xFF, g * 0xFF, b * 0xFF);
	}

	void ToFloat(float& r, float& g, float& b) const;

	bool operator==(const Rgb8& other) const;
};

SDL_Rect RectToSDLRect(const Rect& rect);
Rect SDLRectToRect(const SDL_Rect& sdl_rect);