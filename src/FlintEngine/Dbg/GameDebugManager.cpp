#include "GameDebugManager.h"

namespace debug
{
	GameDebugManager::GameDebugManager(GameBase& game, SceneRenderer* scene_renderer)
		:  m_DebugNavmeshQuerier(game)
		, m_DebugNavmeshLineContainChecker(game)
		, m_DebugNavmeshDirectedRectContainChecker(game)
	{
		if (scene_renderer != nullptr)
		{
			m_DebugRenderer = std::make_unique<DebugRenderer>(*scene_renderer);
		}

		m_DebugMonitorWindow.Init(m_DebugData);
	}

	void GameDebugManager::Update()
	{
		m_DebugNavmeshQuerier.Update();
		m_DebugNavmeshLineContainChecker.Update();
		m_DebugNavmeshDirectedRectContainChecker.Update();
	}

	void GameDebugManager::Render(SceneRenderer& renderer)
	{
		m_DebugConfigWindow.Render();
		m_DebugMonitorWindow.Render();
		m_DebugNavmeshQuerier.Render(renderer);
		m_DebugNavmeshLineContainChecker.Render(renderer);
		m_DebugNavmeshDirectedRectContainChecker.Render(renderer);
	}

	void GameDebugManager::PostFrame()
	{
		m_DebugData.PostFrame();
	}
}