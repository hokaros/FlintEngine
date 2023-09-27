#include "GameBase.h"

#include "../FlatEngine/InputController.h"

GameBase::GameBase(Window* window)
	: window(window)
{
}

bool GameBase::Run()
{
	InputController* input = InputController::Main();

	int black = 0;
	SDL_Surface* screen = NULL;
	if (window != NULL) {
		screen = window->GetScreen();
		black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	}

	PreRun(); // TODO: move to the start of this function

	int quit = 0;

	timer.NextFrame();

	for (GameObject* go : objectManager.GetAllObjects()) {
		go->Start();
	}

	SetRunning(true);
	// Pêtla gry
	while (!quit) {
		// Nowa klatka
		timer.NextFrame();

		if (input != NULL && !input->Update())
			return false;

		if (input != NULL && input->PressedThisFrame(SDLK_ESCAPE))
			quit = 1;

		//generowanie t³a
		if (screen != NULL)
			SDL_FillRect(screen, NULL, black);

		// Wywo³anie zleconych akcji
		InvokePostponed();

		// Zaktualizowanie stanu gry
		for (GameObject* go : objectManager.GetAllObjects()) {
			go->Update();
		}
		PostObjectsUpdate();

		// Renderowanie obiektów
		if (window != NULL) {
			Render();
			window->Render();
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

	PostRender();
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

void GameBase::PostRender()
{
}

void GameBase::PostObjectsUpdate()
{
}

bool GameBase::ShouldRender(GameObject* gameObject)
{
	return true;
}
