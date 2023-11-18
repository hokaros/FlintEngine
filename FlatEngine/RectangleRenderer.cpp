#include "RectangleRenderer.h"
#include "DebugRenderer.h"

DEFINE_COMPONENT(RectangleRenderer);
DEFINE_FIELD(RectangleRenderer, m_Color);

RectangleRenderer::RectangleRenderer(const Rgb8& color)
	: m_Color(color)
{

}

void RectangleRenderer::Render() 
{
	Rect rect = Rect(m_GameObject->GetPosition(), m_GameObject->GetSize());
	DebugRenderer::DrawRect(rect, m_Color);
}

IUpdateable* RectangleRenderer::Copy() 
{
	return new RectangleRenderer(m_Color);
}