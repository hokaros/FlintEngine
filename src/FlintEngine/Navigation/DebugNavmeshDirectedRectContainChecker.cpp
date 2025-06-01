#include "DebugNavmeshDirectedRectContainChecker.h"

#include <Core/GameBase.h>
#include <Dbg/DebugConfig.h>
#include <Navigation/Navmesh.h>

namespace Navigation
{
	DebugNavmeshDirectedRectContainChecker::DebugNavmeshDirectedRectContainChecker(GameBase& game)
		: debug::TwoPointDebugger(game)
	{

	}

	bool DebugNavmeshDirectedRectContainChecker::IsEnabled() const
	{
		return debug::DebugConfig::GetDebugMode() == debug::DebugMode::NavmeshDirectedRectContainment;
	}

	void DebugNavmeshDirectedRectContainChecker::OnStartAndEndPointsSelected(const Vector& start_point, const Vector& end_point)
	{
		m_Rect = DirectedRect(Segment(start_point, end_point), s_RectWidth);
		m_IsRectOnNavmesh = IsRectOnNavmesh(m_Rect);
	}

	void DebugNavmeshDirectedRectContainChecker::RenderWhenStartAndEndPointsSelected(SceneRenderer& renderer)
	{
		Rgb8 color = Rgb8(0xFF, 0x00, 0x00);

		if (m_IsRectOnNavmesh)
		{
			color = Rgb8(0x00, 0xFF, 0x00);
		}

		renderer.RenderRect(m_Rect, color, rendering::LayerId::DEBUG_ABOVE_GROUND);
	}

	bool DebugNavmeshDirectedRectContainChecker::IsRectOnNavmesh(const DirectedRect& rect) const
	{
		if (const Navmesh* navmesh = GetNavmesh())
		{
			return navmesh->ContainsRect(rect);
		}

		return false;
	}

	const Navmesh* DebugNavmeshDirectedRectContainChecker::GetNavmesh() const
	{
		if (const Scene* scene = GetGame().GetCurrentScene())
		{
			return &scene->GetNavmesh();
		}

		return nullptr;
	}
}