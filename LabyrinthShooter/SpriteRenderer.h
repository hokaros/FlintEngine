#pragma once
#include "ObjectRenderer.h"
#include "Window.h"

class SpriteRenderer :
	public ObjectRenderer
{
public:
	SpriteRenderer(GameObject& owner, SDL_Surface* screen, SDL_Surface* sprite);
	~SpriteRenderer();

	void Render(SDL_Surface* screen) override;

	ObjectComponent* Copy(GameObject& newOwner) override;
protected:
	SDL_Surface* sprite;
	SDL_Texture* texture;
};

