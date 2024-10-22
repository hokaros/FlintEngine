#include "Walkable.h"

#include <SceneRenderer.h>
#include <Core/GameObject.h>

DEFINE_COMPONENT(Walkable);
DEFINE_FIELD(Walkable, m_Size);

void Walkable::RenderEditorSelected(SceneRenderer& renderer)
{
	const Rgb8 color = Rgb8(0x30, 0x83, 0xdb);
	const Rect rect = Rect(m_GameObject->GetWorldPosition(), m_Size);
	// TODO: wire rect
	renderer.RenderRect(rect, color, 0);
}

const Vector& Walkable::GetSize() const
{
	return m_Size;
}