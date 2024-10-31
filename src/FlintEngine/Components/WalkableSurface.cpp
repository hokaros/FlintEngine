#include "WalkableSurface.h"

#include <SceneRenderer.h>
#include <Core/GameObject.h>

DEFINE_COMPONENT(WalkableSurface);
DEFINE_FIELD(WalkableSurface, m_Size);

void WalkableSurface::RenderEditorSelected(SceneRenderer& renderer)
{
	const Rgb8 color = Rgb8(0x30, 0x83, 0xdb);
	const Rect rect = Rect(m_GameObject->GetWorldPosition(), m_Size);
	renderer.RenderWireRect(rect, color, 0);
}

const Vector& WalkableSurface::GetSize() const
{
	return m_Size;
}