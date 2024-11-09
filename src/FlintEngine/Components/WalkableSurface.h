#pragma once
#include <ComponentDefinition.h>
#include <Math/GeometryStructures.h>

class WalkableSurface
	: public ObjectComponent
{
	DECLARE_COMPONENT();
public:
	virtual void RenderEditorSelected(SceneRenderer& renderer) override;

	const Vector& GetSize() const;
	void GetVertices(std::vector<Vector>& out_vertices, std::vector<IndexPair>& out_edges) const;

private:
	Vector m_Size;
	DECLARE_FIELD(m_Size);
};