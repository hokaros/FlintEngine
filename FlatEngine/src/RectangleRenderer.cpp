#include "RectangleRenderer.h"
#include "Window.h"
#include "SceneRenderer.h"
#include "GameObject.h"

DEFINE_COMPONENT(RectangleRenderer);
DEFINE_FIELD(RectangleRenderer, m_Color);

RectangleRenderer::RectangleRenderer(const Rgb8& color)
	: m_Color(color)
{

}

void RectangleRenderer::Render(SceneRenderer& renderer)
{
	Rect rect = Rect(m_GameObject->GetPosition(), m_GameObject->GetSize());
	renderer.RenderRect(rect, m_Color);
}

void RectangleRenderer::SetColor(Rgb8 color)
{
	m_Color = color;
}