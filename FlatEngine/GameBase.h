#pragma once
#include "Window.h"
#include "Timer.h"
#include "Draw.h"
#include "ObjectManager.h"
#include "PhysicsSystem.h"
#include "DebugRenderer.h"
#include "DebugConfig.h"


class GameBase
{
public:
	GameBase(Window* window);

	// G³ówna pêtla gry. Zwraca fa³sz, jeœli w trakcie u¿ytkownik zamknie okno
	bool Run();
	bool IsRunning();

	// Usuwa wszystkie obiekty
	void Clear();

	void InvokeOnNextFrame(function<void()> fun);

protected:
	Window* window = nullptr;
	Timer timer;
	ObjectManager objectManager;
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

	virtual void PreRun();
	virtual void PostRun();
	virtual void PrePresent();
	virtual void PostObjectsUpdate();
	virtual bool ShouldRender(GameObject* gameObject);
};

