#include "OccludableRectangle.h"

DEFINE_COMPONENT_DERIVED(OccludableRectangle, RectangleRenderer);

void OccludableRectangle::Start()
{
	OcclusionSystem::RegisterOccludable(*this);
}

void OccludableRectangle::Render(SceneRenderer& renderer)
{
	if (m_RenderingEnabled)
	{
		RectangleRenderer::Render(renderer);
	}
}

void OccludableRectangle::OnDestroy()
{
	OcclusionSystem::UnregisterOccludable(*this);
}

GameObject& OccludableRectangle::GetOwnerObject()
{
	return GetOwner();
}

void OccludableRectangle::EnableRendering()
{
	m_RenderingEnabled = true;
}

void OccludableRectangle::DisableRendering()
{
	m_RenderingEnabled = false;
}
