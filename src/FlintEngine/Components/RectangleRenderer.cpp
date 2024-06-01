#include "RectangleRenderer.h"
#include "Window.h"
#include "SceneRenderer.h"
#include <Core/GameObject.h>

DEFINE_COMPONENT(RectangleRenderer);
DEFINE_FIELD(RectangleRenderer, m_Layer);
DEFINE_FIELD(RectangleRenderer, m_Color);

void RectangleRenderer::Render(SceneRenderer& renderer)
{
	Rect rect = Rect(m_GameObject->GetPosition(), m_GameObject->GetSize());
	renderer.RenderRect(rect, m_Color, m_Layer);
}

void RectangleRenderer::SetColor(Rgb8 color)
{
	m_Color = color;
}

const Rgb8& RectangleRenderer::GetColor() const
{
	return m_Color;
}
