#include "DebugNavmeshQuerier.h"

#include <Core/GameBase.h>

namespace Navigation
{
	DebugNavmeshQuerier::DebugNavmeshQuerier(GameBase& game)
		: m_Game(game)
	{
	}

	void DebugNavmeshQuerier::Update()
	{
		SceneRenderer* scene_renderer = m_Game.GetSceneRenderer();
		if (scene_renderer == nullptr)
			return;

		const IInputController& input = m_Game.GetInput();
		ProcessInput(input, *scene_renderer);
	}

	void DebugNavmeshQuerier::OnPointSelected(const Vector& world_pos)
	{
		switch (m_State)
		{
		case State::NoPointSelected:
			m_StartPoint = world_pos;
			m_State = State::StartPointSelected;
			break;

		case State::StartPointSelected:
			m_EndPoint = world_pos;
			m_State = State::StartAndEndPointsSelected;
			// TODO: calculate path
			break;

		case State::StartAndEndPointsSelected:
			m_State = State::NoPointSelected;
			break;
		}
	}

	void DebugNavmeshQuerier::ProcessInput(const IInputController& input, const SceneRenderer& scene_renderer)
	{
		if (input.MouseButtonPressedThisFrame(MouseButton::Left))
		{
			const Vector mouse_pos = input.GetMousePosition();

			const Vector world_pos = scene_renderer.ScreenSpaceToWorldSpace(mouse_pos);
			OnPointSelected(world_pos);
		}
	}

	void DebugNavmeshQuerier::Render(SceneRenderer& renderer)
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
			RenderPath(renderer);
			break;
		}
	}

	void DebugNavmeshQuerier::RenderPoint(const Vector& world_pos, SceneRenderer& renderer)
	{
		constexpr float x_size = 10.0f;
		constexpr float half_x_size = x_size / 2.0f;

		constexpr uint layer = 1;
		constexpr Rgb8 color = Rgb8(0xFF, 0x00, 0xFF);
		renderer.RenderLine(world_pos - Vector(half_x_size, half_x_size), world_pos + Vector(half_x_size, half_x_size), color, layer);
		renderer.RenderLine(world_pos - Vector(-half_x_size, half_x_size), world_pos + Vector(-half_x_size, half_x_size), color, layer);
	}

	void DebugNavmeshQuerier::RenderPath(SceneRenderer& renderer)
	{
		renderer.RenderLine(m_StartPoint, m_EndPoint, Rgb8(0x00, 0x00, 0xFF), 10);
	}
}