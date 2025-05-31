#include "GameDebugManager.h"

namespace debug
{
	GameDebugManager::GameDebugManager(GameBase& game, SceneRenderer* scene_renderer)
		: m_SceneRenderer(scene_renderer)
		, m_DebugNavmeshQuerier(game)
		, m_DebugNavmeshLineContainChecker(game)
		, m_DebugNavmeshDirectedRectContainChecker(game)
	{
		if (m_SceneRenderer != nullptr)
		{
			m_DebugRenderer = std::make_unique<DebugRenderer>(*m_SceneRenderer);
		}

		m_DebugMonitorWindow.Init(m_DebugData);
	}

	void GameDebugManager::Update()
	{
		m_DebugNavmeshQuerier.Update();
		m_DebugNavmeshLineContainChecker.Update();
		m_DebugNavmeshDirectedRectContainChecker.Update();
	}

	void GameDebugManager::Render()
	{
		m_DebugConfigWindow.Render();
		m_DebugMonitorWindow.Render();
		m_DebugNavmeshQuerier.Render(*m_SceneRenderer);
		m_DebugNavmeshLineContainChecker.Render(*m_SceneRenderer);
		m_DebugNavmeshDirectedRectContainChecker.Render(*m_SceneRenderer);
	}

	void GameDebugManager::PostFrame()
	{
		m_DebugData.PostFrame();
	}
}