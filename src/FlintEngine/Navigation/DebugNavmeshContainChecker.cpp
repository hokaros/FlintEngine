#include "DebugNavmeshContainChecker.h"

#include <Core/GameBase.h>
#include <Dbg/DebugConfig.h>
#include <Navigation/Navmesh.h>

namespace Navigation
{
	DebugNavmeshContainChecker::DebugNavmeshContainChecker(GameBase& game)
		: debug::TwoPointDebugger(game)
	{

	}

	bool DebugNavmeshContainChecker::IsEnabled() const
	{
		return debug::DebugConfig::GetDebugMode() == debug::DebugMode::NavmeshLineContainment;
	}

	void DebugNavmeshContainChecker::OnStartAndEndPointsSelected(const Vector& start_point, const Vector& end_point)
	{
		m_Line = Segment(start_point, end_point);
		m_IsLineOnNavmesh = IsLineOnNavmesh(m_Line);
	}

	void DebugNavmeshContainChecker::RenderWhenStartAndEndPointsSelected(SceneRenderer& renderer)
	{
		constexpr uint layer = 1;
		Rgb8 color = Rgb8(0xFF, 0x00, 0x00);

		if (m_IsLineOnNavmesh)
		{
			color = Rgb8(0x00, 0xFF, 0x00);
		}

		renderer.RenderLine(m_Line.start, m_Line.end, color, layer);
	}

	bool DebugNavmeshContainChecker::IsLineOnNavmesh(const Segment& line) const
	{
		if (const Navmesh* navmesh = GetNavmesh())
		{
			return navmesh->ContainsLine(line);
		}

		return false;
	}

	const Navmesh* DebugNavmeshContainChecker::GetNavmesh() const
	{
		if (const Scene* scene = GetGame().GetCurrentScene())
		{
			return &scene->GetNavmesh();
		}

		return nullptr;
	}
}