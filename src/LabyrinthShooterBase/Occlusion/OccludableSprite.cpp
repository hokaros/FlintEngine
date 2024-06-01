#include "OccludableSprite.h"

DEFINE_COMPONENT_DERIVED(OccludableSprite, SpriteRenderer);

void OccludableSprite::Start()
{
	OcclusionSystem::RegisterOccludable(*this);
}

void OccludableSprite::Render(SceneRenderer& renderer)
{
	if (m_RenderingEnabled)
	{
		SpriteRenderer::Render(renderer);
	}
}

void OccludableSprite::OnDestroy()
{
	OcclusionSystem::UnregisterOccludable(*this);
}

GameObject& OccludableSprite::GetOwnerObject()
{
	return GetOwner();
}

void OccludableSprite::EnableRendering()
{
	m_RenderingEnabled = true;
}

void OccludableSprite::DisableRendering()
{
	m_RenderingEnabled = false;
}
