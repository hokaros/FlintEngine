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

void RectangleRenderer::Render() 
{
	Rect rect = Rect(m_GameObject->GetPosition(), m_GameObject->GetSize());
	SceneRenderer::Main()->RenderRect(rect, m_Color);
}

std::unique_ptr<ObjectComponent> RectangleRenderer::Copy()
{
	return std::make_unique<RectangleRenderer>(m_Color);
}