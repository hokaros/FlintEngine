#include "RectangleRenderer.h"
#include "Window.h"
#include "SceneRenderer.h"
#include <Core/GameObject.h>

DEFINE_COMPONENT(RectangleRenderer);
DEFINE_FIELD(RectangleRenderer, m_Layer);
DEFINE_FIELD(RectangleRenderer, m_Color);
DEFINE_FIELD(RectangleRenderer, m_Size);

void RectangleRenderer::Render(SceneRenderer& renderer)
{
	renderer.RenderRect(GetRenderRect(), m_Color, m_Layer); // TODO: rotation
}

void RectangleRenderer::SetColor(Rgb8 color)
{
	m_Color = color;
}

const Rgb8& RectangleRenderer::GetColor() const
{
	return m_Color;
}

void RectangleRenderer::SetSize(const Vector& size)
{
	m_Size = size;
}

const Vector& RectangleRenderer::GetSize() const
{
	return m_Size;
}

std::vector<VectorInt> RectangleRenderer::GetPixels() const
{
	std::vector<VectorInt> pixels;

	const Rect rect = GetRenderRect();
	const Vector& size = rect.size;
	const Vector& position = rect.pos;

	for (int x = 0; x < size.x; x++)
	{
		for (int y = 0; y < size.y; y++)
		{
			pixels.push_back(position + Vector(x, y));
		}
	}

	return pixels;
}

Vector RectangleRenderer::GetRenderSize() const
{
	return m_Size.GetScaled(m_GameObject->GetWorldScale());
}

Rect RectangleRenderer::GetRenderRect() const
{
	const Vector size = GetRenderSize();
	const Vector pos = m_GameObject->GetWorldPosition() - size / 2.f;
	return Rect(pos, size);
}
