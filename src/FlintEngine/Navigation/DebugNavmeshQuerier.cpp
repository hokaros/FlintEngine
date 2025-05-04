#include "DebugNavmeshQuerier.h"

#include <Core/GameBase.h>
#include <Dbg/DebugConfig.h>

namespace Navigation
{
	DebugNavmeshQuerier::DebugNavmeshQuerier(GameBase& game)
		: m_Game(game)
	{
	}

	void DebugNavmeshQuerier::Update()
	{
		if (debug::DebugConfig::GetDebugMode() != debug::DebugMode::NavmeshQuery)
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

	void DebugNavmeshQuerier::OnPointSelected(const Vector& world_pos)
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
			RenderPath(renderer, m_Path);
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
		if (const Scene* scene = m_Game.GetCurrentScene())
		{
			return &scene->GetNavmesh();
		}

		return nullptr;
	}

	void DebugNavmeshQuerier::CalculatePath(const Navmesh& navmesh, NavmeshPath& path) const
	{
		NavmeshPathfinder::FindPath(navmesh, m_StartPoint, m_EndPoint, path);
	}

	void DebugNavmeshQuerier::Reset()
	{
		m_State = State::NoPointSelected;
	}

	void DebugNavmeshQuerier::OnStartPointSelected(const Vector& world_pos)
	{
		m_StartPoint = world_pos;
		m_State = State::StartPointSelected;
	}

	void DebugNavmeshQuerier::OnEndPointSelected(const Vector& world_pos)
	{
		m_EndPoint = world_pos;

		m_Path.Clear();
		if (const Navmesh* navmesh = GetNavmesh())
		{
			CalculatePath(*navmesh, m_Path);
		}

		m_State = State::StartAndEndPointsSelected;
	}
}