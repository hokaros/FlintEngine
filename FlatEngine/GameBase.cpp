#include "GameBase.h"

#include "InputController.h"

GameBase::GameBase(Window* window)
	: window(window)
	, physicsSystem({})
{
}

bool GameBase::Run()
{
	InputController* input = InputController::Main();

	PreRun(); // TODO: move to the start of this function

	int quit = 0;

	timer.NextFrame();

	SetRunning(true);
	// Pêtla gry
	while (!quit) 
	{
		objectManager.ActivateNewObjects();
		// Nowa klatka
		timer.NextFrame();

		if (input != nullptr && !input->Update())
			return false;

		if (input != nullptr && input->PressedThisFrame(SDLK_ESCAPE))
			quit = 1;

		// Wywo³anie zleconych akcji
		InvokePostponed();

		// Zaktualizowanie stanu gry
		for (GameObject* go : objectManager.GetAllObjects()) {
			go->Update();
		}
		PostObjectsUpdate();

		physicsSystem.Update();

		// Renderowanie obiektów
		if (window != nullptr) 
		{
			Render();
			DebugRender();

			PrePresent();
			window->Present();
		}

		objectManager.DisposeDestroyed();
	}

	SetRunning(false);
	PostRun();

	return true;
}

bool GameBase::IsRunning()
{
	std::lock_guard<std::mutex> lock(metadataMutex);
	return isRunning;
}

void GameBase::Clear()
{
	objectManager.Clear();
}

void GameBase::InvokeOnNextFrame(function<void()> fun)
{
	std::lock_guard<std::mutex> lock(invokesMutex);
	invokes.push_back(std::move(fun));
}

void GameBase::Render()
{
	for (GameObject* go : objectManager.GetAllObjects()) {
		if (go->renderUnseen) {
			go->RenderUpdate();
			continue;
		}

		// TODO: we can refactor vision a lot
		bool canSee = ShouldRender(go);
		if (canSee)
		{
			go->RenderUpdate();
		}
	}
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

void GameBase::PreRun()
{
}

void GameBase::PostRun()
{
}

void GameBase::PrePresent()
{
}

void GameBase::PostObjectsUpdate()
{
}

bool GameBase::ShouldRender(GameObject* gameObject)
{
	return true;
}
