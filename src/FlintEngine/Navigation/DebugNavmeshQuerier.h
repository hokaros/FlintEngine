#pragma once
#include <Navigation/NavmeshPathfinder.h>
#include <Dbg/TwoPointDebugger.h>

namespace Navigation
{
	class DebugNavmeshQuerier
		: public debug::TwoPointDebugger
	{
	public:
		DebugNavmeshQuerier(GameBase& game);

	protected:
		virtual bool IsEnabled() const override;
		virtual void OnStartAndEndPointsSelected(const Vector& start_point, const Vector& end_point) override;
		virtual void RenderWhenStartAndEndPointsSelected(SceneRenderer& renderer) override;

	private:
		void RenderPath(SceneRenderer& renderer, const NavmeshPath& path);

		const Navmesh* GetNavmesh() const;
		void CalculatePath(const Vector& start_point, const Vector& end_point, const Navmesh& navmesh, NavmeshPath& path) const;

	private:
		NavmeshPath m_Path;
	};
}