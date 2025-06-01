#pragma once
#include <Dbg/DebugRenderer.h>
#include <Dbg/DebugConfig.h>
#include <Dbg/DebugMonitor.h>
#include <Dbg/DebugOverlayText.h>
#include <Dbg/MousePositionDebugger.h>
#include <Navigation/DebugNavmeshQuerier.h>
#include <Navigation/DebugNavmeshLineContainChecker.h>
#include <Navigation/DebugNavmeshDirectedRectContainChecker.h>

namespace debug
{
	class GameDebugManager
	{
	public:
		GameDebugManager(GameBase& game, SceneRenderer* scene_renderer);

		DebugOverlayText& GetOverlayText();

		void PreUpdate();
		void Update();
		void Render(SceneRenderer& renderer);
		void PostFrame();

	private:
		std::unique_ptr<DebugRenderer> m_DebugRenderer = nullptr; // TODO: make the DebugRenderer react to SceneRenderer changes

		DebugGameData m_DebugData;
		DebugConfigWindow m_DebugConfigWindow;
		DebugMonitor m_DebugMonitorWindow;
		DebugOverlayText m_DebugOverlayText;
		MousePositionDebugger m_MousePositionDebugger;
		Navigation::DebugNavmeshQuerier m_DebugNavmeshQuerier;
		Navigation::DebugNavmeshLineContainChecker m_DebugNavmeshLineContainChecker;
		Navigation::DebugNavmeshDirectedRectContainChecker m_DebugNavmeshDirectedRectContainChecker;
	};
}