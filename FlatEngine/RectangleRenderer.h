#pragma once
#include "ObjectRenderer.h"
#include "Draw.h"
class RectangleRenderer :
	public ObjectRenderer
{
public:
	RectangleRenderer(GameObject& owner, SDL_Surface* screen, Uint32 outlineColor, Uint32 fillColor);
	void Render(SDL_Surface* screen) override;

	ObjectComponent* Copy(GameObject& newOwner) override;
private:
	Uint32 outlineColor;
	Uint32 fillColor;
};

