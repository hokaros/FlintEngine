#pragma once
#include "ObjectRenderer.h"
#include "Draw.h"
class RectangleRenderer :
	public ObjectRenderer
{
public:
	RectangleRenderer(GameObject& owner, SDL_Surface* screen, const Rgb8& color);
	void Render(SDL_Surface* screen) override;

	ObjectComponent* Copy(GameObject& newOwner) override;
private:
	Rgb8 m_Color;
};

