#pragma once
#include "ObjectRenderer.h"
#include "ComponentDefinition.h"
#include "Draw.h"
#include "SceneRenderer.h"

class RectangleRenderer :
	public ObjectRenderer
{
	DECLARE_COMPONENT();

public:
	RectangleRenderer() = default;
	RectangleRenderer(const Rgb8& color);
	void Render(SceneRenderer& renderer) override;

	void SetColor(Rgb8 color);

private:
	uint m_Layer = 0;
	DECLARE_FIELD(m_Layer);

	Rgb8 m_Color;
	DECLARE_FIELD(m_Color);
};

