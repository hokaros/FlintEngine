#include "RectangleRenderer.h"
#include "DebugRenderer.h"

RectangleRenderer::RectangleRenderer(GameObject& owner, const Rgb8& color)
	:ObjectRenderer(owner)
	, m_Color(color)
{

}

void RectangleRenderer::Render() 
{
	DebugRenderer::DrawRect(gameObject.GetPosition(), gameObject.GetSize(), m_Color);
}

ObjectComponent* RectangleRenderer::Copy(GameObject& newOwner) 
{
	return new RectangleRenderer(newOwner, m_Color);
}