#include "RectangleRenderer.h"
#include "DebugRenderer.h"

DEFINE_COMPONENT(RectangleRenderer);
DEFINE_FIELD(RectangleRenderer, m_Color);

RectangleRenderer::RectangleRenderer(GameObject& owner)
	: ObjectRenderer(owner)
{
}

RectangleRenderer::RectangleRenderer(GameObject& owner, const Rgb8& color)
	:ObjectRenderer(owner)
	, m_Color(color)
{

}

void RectangleRenderer::Render() 
{
	Rect rect = Rect(gameObject.GetPosition(), gameObject.GetSize());
	DebugRenderer::DrawRect(rect, m_Color);
}

ObjectComponent* RectangleRenderer::Copy(GameObject& newOwner) 
{
	return new RectangleRenderer(newOwner, m_Color);
}