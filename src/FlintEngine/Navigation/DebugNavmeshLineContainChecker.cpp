#include "DebugNavmeshLineContainChecker.h"

#include <Core/GameBase.h>
#include <Dbg/DebugConfig.h>
#include <Navigation/Navmesh.h>

namespace Navigation
{
	DebugNavmeshLineContainChecker::DebugNavmeshLineContainChecker(GameBase& game)
		: debug::TwoPointDebugger(game)
	{

	}

	bool DebugNavmeshLineContainChecker::IsEnabled() const
	{
		return debug::DebugConfig::GetDebugMode() == debug::DebugMode::NavmeshLineContainment;
	}

	void DebugNavmeshLineContainChecker::OnStartAndEndPointsSelected(const Vector& start_point, const Vector& end_point)
	{
		m_Line = Segment(start_point, end_point);
		m_IsLineOnNavmesh = IsLineOnNavmesh(m_Line);
	}

	void DebugNavmeshLineContainChecker::RenderWhenStartAndEndPointsSelected(SceneRenderer& renderer)
	{
		constexpr uint layer = 1;
		Rgb8 color = Rgb8(0xFF, 0x00, 0x00);

		if (m_IsLineOnNavmesh)
		{
			color = Rgb8(0x00, 0xFF, 0x00);
		}

		renderer.RenderLine(m_Line.start, m_Line.end, color, layer);
	}

	bool DebugNavmeshLineContainChecker::IsLineOnNavmesh(const Segment& line) const
	{
		if (const Navmesh* navmesh = GetNavmesh())
		{
			return navmesh->ContainsLine(line);
		}

		return false;
	}

	const Navmesh* DebugNavmeshLineContainChecker::GetNavmesh() const
	{
		if (const Scene* scene = GetGame().GetCurrentScene())
		{
			return &scene->GetNavmesh();
		}

		return nullptr;
	}
}