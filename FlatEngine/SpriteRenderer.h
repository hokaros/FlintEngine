#pragma once
#include "ObjectRenderer.h"
#include "Window.h"

class SpriteRenderer :
	public ObjectRenderer
{
public:
	SpriteRenderer(SDL_Surface* sprite);
	~SpriteRenderer();

	void Render() override;

	IUpdateable* Copy() override;
protected:
	SDL_Surface* m_Sprite;
	SDL_Texture* m_Texture;
};

