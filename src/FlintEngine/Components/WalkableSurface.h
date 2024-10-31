#pragma once
#include <ComponentDefinition.h>

class WalkableSurface
	: public ObjectComponent
{
	DECLARE_COMPONENT();
public:
	virtual void RenderEditorSelected(SceneRenderer& renderer) override;

	const Vector& GetSize() const;

private:
	Vector m_Size;
	DECLARE_FIELD(m_Size);
};