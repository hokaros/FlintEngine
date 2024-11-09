#include "WalkableSurface.h"

#include <SceneRenderer.h>
#include <Core/GameObject.h>

DEFINE_COMPONENT(WalkableSurface);
DEFINE_FIELD(WalkableSurface, m_Size);

void WalkableSurface::RenderEditorSelected(SceneRenderer& renderer)
{
	const Rgb8 color = Rgb8(0x30, 0x83, 0xdb);
	const Rect rect = Rect(m_GameObject->GetWorldPosition() - m_Size / 2.0f, m_Size);
	renderer.RenderWireRect(rect, color, 0);
}

const Vector& WalkableSurface::GetSize() const
{
	return m_Size;
}

void WalkableSurface::GetVertices(std::vector<Vector>& out_vertices, std::vector<IndexPair>& out_edges) const
{
	const Vector top_left = Vector(-m_Size.x, -m_Size.y) / 2.f;
	const Vector top_right = top_left + Vector(m_Size.x, 0);
	const Vector bottom_left = top_left + Vector(0, m_Size.y);
	const Vector bottom_right = bottom_left + Vector(m_Size.x, 0);

	const GameObject& owner = GetOwner();

	out_vertices.push_back(owner.TransformPoint(top_left));
	const size_t top_left_idx = out_vertices.size() - 1;

	out_vertices.push_back(owner.TransformPoint(top_right));
	const size_t top_right_idx = out_vertices.size() - 1;

	out_vertices.push_back(owner.TransformPoint(bottom_left));
	const size_t bottom_left_idx = out_vertices.size() - 1;

	out_vertices.push_back(owner.TransformPoint(bottom_right));
	const size_t bottom_right_idx = out_vertices.size() - 1;

	out_edges.push_back({ top_left_idx, top_right_idx });
	out_edges.push_back({ top_left_idx, bottom_left_idx });
	out_edges.push_back({ bottom_left_idx, bottom_right_idx });
	out_edges.push_back({ bottom_right_idx, top_right_idx });
}