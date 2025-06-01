#pragma once
#include "Window.h"
#include "Timer.h"
#include "Draw.h"
#include "ObjectManager.h"
#include "PhysicsSystem.h"
#include <Scene/Scene.h>
#include "InputController.h"
#include <Dbg/GameDebugManager.h>

class GameBase
{
public:
	GameBase(Window* window, SceneRenderer* scene_renderer, IInputController& input_controller);
	virtual ~GameBase();

	const SceneRenderer* GetSceneRenderer() const;
	SceneRenderer* GetSceneRenderer();
	const IInputController& GetInput() const;

	// G³ówna pêtla gry. Zwraca fa³sz, jeœli w trakcie u¿ytkownik zamknie okno
	bool Run();
	bool RunOneLoop();
	bool IsRunning();

	void LoadScene(std::unique_ptr<Scene> scene);
	Scene* GetCurrentScene();

	void InvokeOnNextFrame(function<void()> fun);

	static GameBase* GetCurrent();

protected:
	IInputController& m_InputController;
	Window* m_Window = nullptr;
	SceneRenderer* m_SceneRenderer = nullptr;

	Timer m_Timer;
	PhysicsSystem m_PhysicsSystem;

	std::unique_ptr<Scene> m_CurrScene = nullptr;

	bool m_IsRunning = false;

	// Lista funkcji do wykonania w najbli¿szej klatce
	std::list<function<void()>> m_Invokes;

	std::mutex m_InvokesMutex;
	std::mutex m_MetadataMutex;

	debug::GameDebugManager m_DebugManager;

private:
	void DebugRender();
	void PostFrameSleep();

	void InvokePostponed();

	void SetRunning(bool running);

	double GetMinSecondsForFrame() const;

private:
	static constexpr float s_FrameRateClamp = 60.f;

	static GameBase* s_Current;
};

