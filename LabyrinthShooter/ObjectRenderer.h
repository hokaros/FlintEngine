#pragma once
#include <SDL.h>
#include "ObjectComponent.h"

// Komponent rysuj¹cy obiekt
class ObjectRenderer : public ObjectComponent
{
public:
	ObjectRenderer(GameObject& owner, SDL_Surface* screen);
	void RenderUpdate() override;
	virtual void Render(SDL_Surface* screen) = 0;
protected:
	SDL_Surface* screen;
};

