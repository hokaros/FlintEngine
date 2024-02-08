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
	SpriteRenderer(const std::string& bitmap_path);
	~SpriteRenderer();

	void Awake() override;
	void Render() override;

	virtual std::unique_ptr<ObjectComponent> Copy() override;
protected:
	std::string m_BitmapPath = "";

	SDL_Texture* m_Texture = nullptr;
};

