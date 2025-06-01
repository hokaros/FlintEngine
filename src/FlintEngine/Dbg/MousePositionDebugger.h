#pragma once

class SceneRenderer;

namespace debug
{
	class GameDebugManager;

	class MousePositionDebugger
	{
	public:
		MousePositionDebugger(GameDebugManager& debug_manager);

		void Update();

	private:
		GameDebugManager& m_DebugManager;
	};
}