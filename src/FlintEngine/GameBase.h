#pragma once
#include "Window.h"
#include "Timer.h"
#include "Draw.h"
#include "ObjectManager.h"
#include "PhysicsSystem.h"
#include "DebugRenderer.h"
#include "DebugConfig.h"
#include "Scene.h"
#include "InputController.h"


class GameBase
{
public:
	GameBase(Window* window, SceneRenderer* scene_renderer, IInputController& input_controller);

	// G³ówna pêtla gry. Zwraca fa³sz, jeœli w trakcie u¿ytkownik zamknie okno
	bool Run();
	bool RunOneLoop();
	bool IsRunning();

	void LoadScene(std::unique_ptr<Scene> scene);

	void InvokeOnNextFrame(function<void()> fun);

protected:
	IInputController& m_InputController;
	Window* m_Window = nullptr;
	SceneRenderer* m_SceneRenderer = nullptr;
	std::unique_ptr<DebugRenderer> m_DebugRenderer = nullptr;

	Timer timer;
	PhysicsSystem physicsSystem;
	debug::DebugConfigWindow m_DebugConfigWindow;

	std::unique_ptr<Scene> m_CurrScene = nullptr;

	bool isRunning = false;

	// Lista funkcji do wykonania w najbli¿szej klatce
	std::list<function<void()>> invokes;

	std::mutex invokesMutex;
	std::mutex metadataMutex;

private:
	void DebugRender();

	void InvokePostponed();

	void SetRunning(bool running);
};

