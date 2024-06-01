#pragma once
#include <Components/RectangleRenderer.h>
#include "OcclusionSystem.h"

// TODO: how about decorator components?
class OccludableRectangle
	: public RectangleRenderer
	, protected IOccludable
{
	DECLARE_COMPONENT();
public:
	OccludableRectangle() = default;

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