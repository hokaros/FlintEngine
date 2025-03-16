#pragma once
#include <Math/Vector.h>
#include <Navigation/NavmeshPathfinder.h>

class GameBase;
class SceneRenderer;
class IInputController;

namespace Navigation
{
	class DebugNavmeshQuerier
	{
	public:
		DebugNavmeshQuerier(GameBase& game);

		void Update();
		void Render(SceneRenderer& renderer);

	private:
		enum class State
		{
			NoPointSelected,
			StartPointSelected,
			StartAndEndPointsSelected
		};

	private:
		void OnPointSelected(const Vector& world_pos);
		void ProcessInput(const IInputController& input, const SceneRenderer& scene_renderer);

		void RenderPoint(const Vector& world_pos, SceneRenderer& renderer);
		void RenderPath(SceneRenderer& renderer, const NavmeshPath& path);

		const Navmesh* GetNavmesh() const;
		void CalculatePath(const Navmesh& navmesh, NavmeshPath& path) const;

	private:
		GameBase& m_Game;

		State m_State = State::NoPointSelected;
		Vector m_StartPoint;
		Vector m_EndPoint;
		NavmeshPath m_Path;
	};
}