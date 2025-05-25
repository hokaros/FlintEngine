#include "DebugNavmeshQuerier.h"

#include <SceneRenderer.h>
#include <Core/GameBase.h>
#include <Dbg/DebugConfig.h>

namespace Navigation
{
	DebugNavmeshQuerier::DebugNavmeshQuerier(GameBase& game)
		: debug::TwoPointDebugger(game)
	{
	}

	bool DebugNavmeshQuerier::IsEnabled() const
	{
		return debug::DebugConfig::GetDebugMode() == debug::DebugMode::NavmeshQuery;
	}

	void DebugNavmeshQuerier::OnStartAndEndPointsSelected(const Vector& start_point, const Vector& end_point)
	{
		m_Path.Clear();
		if (const Navmesh* navmesh = GetNavmesh())
		{
			CalculatePath(start_point, end_point, *navmesh, m_Path);
		}
	}

	void DebugNavmeshQuerier::RenderWhenStartAndEndPointsSelected(SceneRenderer& renderer)
	{
		RenderPath(renderer, m_Path);
	}

	void DebugNavmeshQuerier::RenderPath(SceneRenderer& renderer, const NavmeshPath& path)
	{
		constexpr uint layer = 1;

		const Vector* prev_point = nullptr;

		for (const Vector& control_point : path)
		{
			if (prev_point != nullptr)
			{
				renderer.RenderLine(control_point, *prev_point, Rgb8(0x00, 0x00, 0xFF), layer);
			}

			prev_point = &control_point;
		}
	}

	const Navmesh* DebugNavmeshQuerier::GetNavmesh() const
	{
		if (const Scene* scene = GetGame().GetCurrentScene())
		{
			return &scene->GetNavmesh();
		}

		return nullptr;
	}

	void DebugNavmeshQuerier::CalculatePath(const Vector& start_point, const Vector& end_point, const Navmesh& navmesh, NavmeshPath& path) const
	{
		NavmeshPathfinder::FindPath(navmesh, start_point, end_point, 0.f, path);
	}
}