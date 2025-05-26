#pragma once
#include <Dbg/TwoPointDebugger.h>
#include <Math/DirectedRect.h>

namespace Navigation
{
	class Navmesh;

	class DebugNavmeshDirectedRectContainChecker
		: public debug::TwoPointDebugger
	{
	public:
		DebugNavmeshDirectedRectContainChecker(GameBase& game);

	protected:
		virtual bool IsEnabled() const override;
		virtual void OnStartAndEndPointsSelected(const Vector& start_point, const Vector& end_point) override;
		virtual void RenderWhenStartAndEndPointsSelected(SceneRenderer& renderer) override;

	private:
		bool IsRectOnNavmesh(const DirectedRect& line) const;
		const Navmesh* GetNavmesh() const;

	private:
		DirectedRect m_Rect;
		bool m_IsRectOnNavmesh = false;

		static constexpr float s_RectWidth = 14.0f;
	};
}