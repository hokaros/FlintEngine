#pragma once
#include <Math/Vector.h>

class GameBase;
class SceneRenderer;
class IInputController;

namespace debug
{
	class TwoPointDebugger
	{
	public:
		TwoPointDebugger(GameBase& game);

		void Update();
		void Render(SceneRenderer& renderer);

		virtual ~TwoPointDebugger() = default;

	protected:
		virtual bool IsEnabled() const = 0;
		virtual void OnStartAndEndPointsSelected(const Vector& start_point, const Vector& end_point) = 0;
		virtual void RenderWhenStartAndEndPointsSelected(SceneRenderer& renderer) = 0;

		GameBase& GetGame() const;

	private:
		enum class State
		{
			NoPointSelected,
			StartPointSelected,
			StartAndEndPointsSelected
		};

	private:
		void ProcessInput(const IInputController& input, const SceneRenderer& scene_renderer);
		void OnPointSelected(const Vector& world_pos);
		void OnStartPointSelected(const Vector& world_pos);
		void OnEndPointSelected(const Vector& world_pos);

		void Reset();

		void RenderPoint(const Vector& world_pos, SceneRenderer& renderer);

	private:
		GameBase& m_Game;

		State m_State = State::NoPointSelected;
		Vector m_StartPoint;
		Vector m_EndPoint;
	};
}