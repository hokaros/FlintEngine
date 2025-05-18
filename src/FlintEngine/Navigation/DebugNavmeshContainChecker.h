#pragma once
#include <Dbg/TwoPointDebugger.h>
#include <Math/Segment.h>

namespace Navigation
{
	class Navmesh;

	class DebugNavmeshContainChecker
		: public debug::TwoPointDebugger
	{
	public:
		DebugNavmeshContainChecker(GameBase& game);

	protected:
		virtual bool IsEnabled() const override;
		virtual void OnStartAndEndPointsSelected(const Vector& start_point, const Vector& end_point) override;
		virtual void RenderWhenStartAndEndPointsSelected(SceneRenderer& renderer) override;

	private:
		bool IsLineOnNavmesh(const Segment& line) const;
		const Navmesh* GetNavmesh() const;

	private:
		Segment m_Line;
		bool m_IsLineOnNavmesh = false;
	};
}