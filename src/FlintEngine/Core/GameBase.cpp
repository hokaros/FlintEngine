#include "GameBase.h"

#include <Windows.h>

GameBase::GameBase(Window* window, SceneRenderer* scene_renderer, IInputController& input_controller)
	: m_Window(window)
	, m_SceneRenderer(scene_renderer)
	, m_PhysicsSystem({})
	, m_InputController(input_controller)
{
	if (m_SceneRenderer != nullptr)
	{
		m_DebugRenderer = std::make_unique<DebugRenderer>(*m_SceneRenderer);
	}

	m_DebugMonitorWindow.Init(m_DebugData);
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

	// Renderowanie obiekt�w
	if (m_SceneRenderer != nullptr)
	{
		m_DebugConfigWindow.Render();
		m_DebugMonitorWindow.Render();

		m_CurrScene->Render(*m_SceneRenderer);
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

void GameBase::InvokeOnNextFrame(function<void()> fun)
{
	std::lock_guard<std::mutex> lock(m_InvokesMutex);
	m_Invokes.push_back(std::move(fun));
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