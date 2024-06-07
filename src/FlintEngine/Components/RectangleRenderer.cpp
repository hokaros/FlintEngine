#include "RectangleRenderer.h"
#include "Window.h"
#include "SceneRenderer.h"
#include <Core/GameObject.h>

DEFINE_COMPONENT(RectangleRenderer);
DEFINE_FIELD(RectangleRenderer, m_Layer);
DEFINE_FIELD(RectangleRenderer, m_Color);

void RectangleRenderer::Render(SceneRenderer& renderer)
{
	const Vector& world_pos = m_GameObject->GetWorldPosition();
	const Vector& world_size = m_GameObject->GetWorldScale();
	const Rect rect = Rect(world_pos, world_size);

	renderer.RenderRect(rect, m_Color, m_Layer); // TODO: rotation
}

void RectangleRenderer::SetColor(Rgb8 color)
{
	m_Color = color;
}

const Rgb8& RectangleRenderer::GetColor() const
{
	return m_Color;
}
