#include "GameBase.h"

GameBase::GameBase(Window* window, SceneRenderer* scene_renderer, IInputController& input_controller)
	: m_Window(window)
	, m_SceneRenderer(scene_renderer)
	, physicsSystem({})
	, m_InputController(input_controller)
{
	if (m_SceneRenderer != nullptr)
	{
		m_DebugRenderer = std::make_unique<DebugRenderer>(*m_SceneRenderer);
	}
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
	timer.NextFrame();

	if (!m_InputController.Update())
		return false;

	if (m_InputController.PressedThisFrame(SDLK_ESCAPE))
		return false;

	// Wywo�anie zleconych akcji
	InvokePostponed();

	// Zaktualizowanie stanu gry
	m_CurrScene->Update();

	physicsSystem.Update();

	// Renderowanie obiekt�w
	if (m_SceneRenderer != nullptr)
	{
		m_DebugConfigWindow.Render();
		m_CurrScene->Render(*m_SceneRenderer);
		DebugRender();

		if (m_Window != nullptr)
		{
			m_Window->Present();
		}
	}

	m_CurrScene->PostFrame();

	return true;
}

bool GameBase::IsRunning()
{
	std::lock_guard<std::mutex> lock(metadataMutex);
	return isRunning;
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
	std::lock_guard<std::mutex> lock(invokesMutex);
	invokes.push_back(std::move(fun));
}

void GameBase::DebugRender()
{
	physicsSystem.DebugRender();
}

void GameBase::InvokePostponed()
{
	std::lock_guard<std::mutex> lock(invokesMutex);
	for (function<void()> fun : invokes) {
		if (fun)
			fun();
	}
	invokes.clear();
}

void GameBase::SetRunning(bool running)
{
	std::lock_guard<std::mutex> lock(metadataMutex);
	isRunning = running;
}
