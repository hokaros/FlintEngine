#pragma once
#include <Components/SpriteRenderer.h>
#include "OcclusionSystem.h"


class OccludableSprite
	: public SpriteRenderer
	, protected IOccludable
{
	DECLARE_COMPONENT();
public:
	OccludableSprite() = default;

	virtual void Start() override;
	virtual void Render(SceneRenderer& renderer) override;
	virtual void OnDestroy() override;

	// IOccludable
	virtual GameObject& GetOwnerObject() override;
	virtual void EnableRendering() override;
	virtual void DisableRendering() override;

private:
	bool m_RenderingEnabled = true;
};