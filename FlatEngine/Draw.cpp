#include "Draw.h"

SDL_Rect RectToSDLRect(const Rect& rect)
{
	SDL_Rect sdl_rect;
	sdl_rect.x = rect.pos.x;
	sdl_rect.y = rect.pos.y;
	sdl_rect.w = rect.size.x;
	sdl_rect.h = rect.size.y;
	return sdl_rect;
}

Rect SDLRectToRect(const SDL_Rect& sdl_rect)
{
	return Rect(
		Vector(sdl_rect.x, sdl_rect.y),
		Vector(sdl_rect.w, sdl_rect.h)
	);
}