#include "GameBase.h"

#include <Windows.h>

GameBase* GameBase::s_Current{ nullptr };

GameBase::GameBase(Window* window, SceneRenderer* scene_renderer, IInputController& input_controller)
	: m_Window(window)
	, m_SceneRenderer(scene_renderer)
	, m_PhysicsSystem({})
	, m_InputController(input_controller)
	, m_DebugNavmeshQuerier(*this)
	, m_DebugNavmeshLineContainChecker(*this)
	, m_DebugNavmeshDirectedRectContainChecker(*this)
{
	if (m_SceneRenderer != nullptr)
	{
		m_DebugRenderer = std::make_unique<DebugRenderer>(*m_SceneRenderer);
	}

	m_DebugMonitorWindow.Init(m_DebugData);

	s_Current = this;
}

GameBase::~GameBase()
{
	if (s_Current == this)
	{
		s_Current = nullptr;
	}
}

const SceneRenderer* GameBase::GetSceneRenderer() const
{
	return m_SceneRenderer;
}

SceneRenderer* GameBase::GetSceneRenderer()
{
	return m_SceneRenderer;
}

const IInputController& GameBase::GetInput() const
{
	return m_InputController;
}

bool GameBase::Run()
{
	FE_ASSERT(m_CurrScene != nullptr, "No scene loaded");

	// P�tla gry
	SetRunning(true);
	bool is_running = true;
	while (is_running) 
	{
		is_running = RunOneLoop();
	}

	SetRunning(false);

	return true;
}

bool GameBase::RunOneLoop()
{
	// Nowa klatka
	m_Timer.FrameStart();

	if (!m_InputController.Update())
		return false;

	if (m_InputController.PressedThisFrame(SDLK_ESCAPE))
		return false;

	// Wywo�anie zleconych akcji
	InvokePostponed();

	// Zaktualizowanie stanu gry
	m_CurrScene->Update();

	m_PhysicsSystem.Update();

	UpdateDebuggers();

	// Renderowanie obiekt�w
	if (m_SceneRenderer != nullptr)
	{
		m_CurrScene->Render(*m_SceneRenderer);

		RenderDebuggers();
		DebugRender();

		if (m_Window != nullptr)
		{
			m_Window->Present();
		}
	}

	m_CurrScene->PostFrame();

	m_DebugData.PostFrame();

	PostFrameSleep();
	return true;
}

bool GameBase::IsRunning()
{
	std::lock_guard<std::mutex> lock(m_MetadataMutex);
	return m_IsRunning;
}

void GameBase::LoadScene(std::unique_ptr<Scene> scene)
{
	m_CurrScene = std::move(scene);

	if (m_CurrScene != nullptr)
	{
		m_CurrScene->OnLoaded();
	}
}

Scene* GameBase::GetCurrentScene()
{
	return m_CurrScene.get();
}

void GameBase::InvokeOnNextFrame(function<void()> fun)
{
	std::lock_guard<std::mutex> lock(m_InvokesMutex);
	m_Invokes.push_back(std::move(fun));
}

GameBase* GameBase::GetCurrent()
{
	return s_Current;
}

void GameBase::UpdateDebuggers()
{
	m_DebugNavmeshQuerier.Update();
	m_DebugNavmeshLineContainChecker.Update();
	m_DebugNavmeshDirectedRectContainChecker.Update();
}

void GameBase::RenderDebuggers()
{
	m_DebugConfigWindow.Render();
	m_DebugMonitorWindow.Render();
	m_DebugNavmeshQuerier.Render(*m_SceneRenderer);
	m_DebugNavmeshLineContainChecker.Render(*m_SceneRenderer);
	m_DebugNavmeshDirectedRectContainChecker.Render(*m_SceneRenderer);
}

void GameBase::DebugRender()
{
	m_PhysicsSystem.DebugRender();
}

void GameBase::InvokePostponed()
{
	std::lock_guard<std::mutex> lock(m_InvokesMutex);
	for (function<void()> fun : m_Invokes) {
		if (fun)
			fun();
	}
	m_Invokes.clear();
}

void GameBase::SetRunning(bool running)
{
	std::lock_guard<std::mutex> lock(m_MetadataMutex);
	m_IsRunning = running;
}

void GameBase::PostFrameSleep()
{
	const double min_seconds_per_frame = GetMinSecondsForFrame();

	const double last_frame_time = m_Timer.GetTimeSinceFrameStart();

	if (last_frame_time < min_seconds_per_frame)
	{
		const size_t sleep_miliseconds = (min_seconds_per_frame - last_frame_time) * 1000.0;
		Sleep(sleep_miliseconds);
	}
}

double GameBase::GetMinSecondsForFrame() const
{
	return 1.0 / s_FrameRateClamp;
}