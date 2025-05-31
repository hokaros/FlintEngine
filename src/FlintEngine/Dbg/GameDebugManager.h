#pragma once
#include <Dbg/DebugRenderer.h>
#include <Dbg/DebugConfig.h>
#include <Dbg/DebugMonitor.h>
#include <Navigation/DebugNavmeshQuerier.h>
#include <Navigation/DebugNavmeshLineContainChecker.h>
#include <Navigation/DebugNavmeshDirectedRectContainChecker.h>

namespace debug
{
	class GameDebugManager
	{
	public:
		GameDebugManager(GameBase& game, SceneRenderer* scene_renderer);

		void Update();
		void Render();
		void PostFrame();

	private:
		std::unique_ptr<DebugRenderer> m_DebugRenderer = nullptr;
		SceneRenderer* m_SceneRenderer = nullptr;

		debug::DebugGameData m_DebugData;
		debug::DebugConfigWindow m_DebugConfigWindow;
		debug::DebugMonitor m_DebugMonitorWindow;
		Navigation::DebugNavmeshQuerier m_DebugNavmeshQuerier;
		Navigation::DebugNavmeshLineContainChecker m_DebugNavmeshLineContainChecker;
		Navigation::DebugNavmeshDirectedRectContainChecker m_DebugNavmeshDirectedRectContainChecker;
	};
}