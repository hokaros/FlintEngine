#include "TwoPointDebugger.h"

#include <Core/GameBase.h>

namespace debug
{
	TwoPointDebugger::TwoPointDebugger(GameBase& game)
		: m_Game(game)
	{
	}

	void TwoPointDebugger::Update()
	{
		if (!IsEnabled())
		{
			Reset();
			return;
		}

		SceneRenderer* scene_renderer = m_Game.GetSceneRenderer();
		if (scene_renderer == nullptr)
			return;

		const IInputController& input = m_Game.GetInput();
		ProcessInput(input, *scene_renderer);
	}

	void TwoPointDebugger::Render(SceneRenderer& renderer)
	{
		switch (m_State)
		{
		case State::NoPointSelected:
			break; // Don't render anything

		case State::StartPointSelected:
			RenderPoint(m_StartPoint, renderer);
			break;

		case State::StartAndEndPointsSelected:
			RenderPoint(m_StartPoint, renderer);
			RenderPoint(m_EndPoint, renderer);
			RenderWhenStartAndEndPointsSelected(renderer);
			break;
		}
	}

	GameBase& TwoPointDebugger::GetGame() const
	{
		return m_Game;
	}

	void TwoPointDebugger::ProcessInput(const IInputController& input, const SceneRenderer& scene_renderer)
	{
		if (input.MouseButtonPressedThisFrame(MouseButton::Left))
		{
			const Vector mouse_pos = input.GetMousePosition();

			const Vector world_pos = scene_renderer.ScreenSpaceToWorldSpace(mouse_pos);
			OnPointSelected(world_pos);
		}
	}

	void TwoPointDebugger::OnPointSelected(const Vector& world_pos)
	{
		switch (m_State)
		{
		case State::NoPointSelected:
			OnStartPointSelected(world_pos);
			break;

		case State::StartPointSelected:
			OnEndPointSelected(world_pos);
			break;

		case State::StartAndEndPointsSelected:
			Reset();
			break;
		}
	}

	void TwoPointDebugger::OnStartPointSelected(const Vector& world_pos)
	{
		m_StartPoint = world_pos;
		m_State = State::StartPointSelected;
	}

	void TwoPointDebugger::OnEndPointSelected(const Vector& world_pos)
	{
		m_EndPoint = world_pos;
		m_State = State::StartAndEndPointsSelected;

		OnStartAndEndPointsSelected(m_StartPoint, m_EndPoint);
	}

	void TwoPointDebugger::Reset()
	{
		m_State = State::NoPointSelected;
	}

	void TwoPointDebugger::RenderPoint(const Vector& world_pos, SceneRenderer& renderer)
	{
		constexpr float x_size = 10.0f;
		constexpr float half_x_size = x_size / 2.0f;

		const rendering::LayerId layer = rendering::LayerId::DEBUG_ABOVE_GROUND;
		constexpr Rgb8 color = Rgb8(0xFF, 0x00, 0xFF);
		renderer.RenderLine(world_pos - Vector(half_x_size, half_x_size), world_pos + Vector(half_x_size, half_x_size), color, layer);
		renderer.RenderLine(world_pos - Vector(-half_x_size, half_x_size), world_pos + Vector(-half_x_size, half_x_size), color, layer);
	}
}
