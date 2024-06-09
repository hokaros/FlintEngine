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
	void Render(SceneRenderer& renderer) override;

	void SetColor(Rgb8 color);
	const Rgb8& GetColor() const;

	void SetSize(const Vector& size);
	const Vector& GetSize() const;

	std::vector<VectorInt> GetPixels() const;

private:
	Vector GetRenderSize() const;
	Rect GetRenderRect() const;

private:
	uint m_Layer = 0;
	DECLARE_FIELD(m_Layer);

	Rgb8 m_Color;
	DECLARE_FIELD(m_Color);

	Vector m_Size;
	DECLARE_FIELD(m_Size);
};

