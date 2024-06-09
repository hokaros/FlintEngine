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
	~SpriteRenderer();

	void Render(SceneRenderer& renderer) override;

protected:
	Vector GetRenderSize() const;
	Rect GetRenderRect() const;

	static SDL_Texture* CreateTextureFromBitmap(const std::string& bitmap_path, SceneRenderer& renderer);

protected:
	uint m_Layer = 0;
	DECLARE_FIELD(m_Layer);

	std::string m_BitmapPath = "";
	DECLARE_FIELD(m_BitmapPath);

	Vector m_Size = Vector(1, 1);
	DECLARE_FIELD(m_Size);

	SDL_Texture* m_Texture = nullptr;
};

