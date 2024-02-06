#include "GameBase.h"

#include "InputController.h"

GameBase::GameBase(Window* window)
	: window(window)
	, physicsSystem({})
{
}

bool GameBase::Run()
{
	std::unique_ptr<Scene> scene = CreateScene();

	InputController* input = InputController::Main();

	int quit = 0;

	timer.NextFrame();

	SetRunning(true);
	// Pêtla gry
	while (!quit) 
	{
		// Nowa klatka
		timer.NextFrame();

		if (input != nullptr && !input->Update())
			return false;

		if (input != nullptr && input->PressedThisFrame(SDLK_ESCAPE))
			quit = 1;

		// Wywo³anie zleconych akcji
		InvokePostponed();

		// Zaktualizowanie stanu gry
		scene->Update();

		physicsSystem.Update();

		// Renderowanie obiektów
		if (window != nullptr) 
		{
			m_DebugConfigWindow.Render();
			scene->Render();
			Render();
			DebugRender();

			window->Present();
		}

		scene->PostFrame();
	}

	SetRunning(false);

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

void GameBase::Render()
{
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
