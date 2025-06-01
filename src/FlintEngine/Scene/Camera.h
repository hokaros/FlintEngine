#pragma once
#include <ComponentDefinition.h>

class Camera
	: public ObjectComponent
{
	DECLARE_COMPONENT();

public:
	Rect CalculateViewport() const;

	virtual void Awake() override;
	virtual void OnDestroy() override;
	virtual void RenderEditorSelected(SceneRenderer& renderer) override;

private:
	Vector m_Size = Vector(800, 600);
	DECLARE_FIELD(m_Size);
};