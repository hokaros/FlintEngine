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
