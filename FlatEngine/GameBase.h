#pragma once
#include "Window.h"
#include "Timer.h"
#include "Draw.h"
#include "ObjectManager.h"
#include "PhysicsSystem.h"
#include "DebugRenderer.h"
#include "DebugConfig.h"
#include "Scene.h"


class GameBase
{
public:
	GameBase(Window* window);

	// G³ówna pêtla gry. Zwraca fa³sz, jeœli w trakcie u¿ytkownik zamknie okno
	bool Run();
	bool IsRunning();

	void InvokeOnNextFrame(function<void()> fun);

protected:
	virtual std::unique_ptr<Scene> CreateScene();

protected:
	Window* window = nullptr;
	Timer timer;
	PhysicsSystem physicsSystem;
	debug::DebugConfigWindow m_DebugConfigWindow;

	bool isRunning = false;

	// Lista funkcji do wykonania w najbli¿szej klatce
	std::list<function<void()>> invokes;

	std::mutex invokesMutex;
	std::mutex metadataMutex;

private:
	void Render();
	void DebugRender();

	void InvokePostponed();

	void SetRunning(bool running);
};

