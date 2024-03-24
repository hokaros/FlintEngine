#include "GameBase.h"

#include "InputController.h"

GameBase::GameBase(Window* window)
	: m_Window(window)
	, physicsSystem({})
{
}

bool GameBase::Run()
{
	m_CurrScene = CreateScene(); // TODO: inject

	timer.NextFrame();

	// Pêtla gry
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

	InputController* input = InputController::Main();
	if (input != nullptr && !input->Update())
		return false;

	if (input != nullptr && input->PressedThisFrame(SDLK_ESCAPE))
		return false;

	// Wywo³anie zleconych akcji
	InvokePostponed();

	// Zaktualizowanie stanu gry
	m_CurrScene->Update();

	physicsSystem.Update();

	// Renderowanie obiektów
	if (m_ShouldRender)
	{
		m_DebugConfigWindow.Render();
		m_CurrScene->Render();
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

void GameBase::InvokeOnNextFrame(function<void()> fun)
{
	std::lock_guard<std::mutex> lock(invokesMutex);
	invokes.push_back(std::move(fun));
}

std::unique_ptr<Scene> GameBase::CreateScene()
{
	return std::make_unique<Scene>();
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
