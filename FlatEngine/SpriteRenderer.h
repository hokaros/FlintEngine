#pragma once
#include "ObjectRenderer.h"
#include "Window.h"
#include "ComponentDefinition.h"

class SpriteRenderer :
	public ObjectRenderer
{
	DECLARE_COMPONENT();
public:
	SpriteRenderer() = default;
	SpriteRenderer(SDL_Surface* sprite);
	~SpriteRenderer();

	void Render() override;

	virtual std::unique_ptr<ObjectComponent> Copy() override;
protected:
	SDL_Surface* m_Sprite = nullptr;
	SDL_Texture* m_Texture = nullptr;
};

