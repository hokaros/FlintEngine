#pragma once
#include "../FlatEngine/Window.h"
#include "../FlatEngine/Timer.h"
#include "../FlatEngine/Draw.h"
#include "../FlatEngine/ObjectManager.h"


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
	Window* window = NULL;
	Timer timer;
	ObjectManager objectManager;

	bool isRunning = false;

	// Lista funkcji do wykonania w najbli¿szej klatce
	std::list<function<void()>> invokes;

	std::mutex invokesMutex;
	std::mutex metadataMutex;

private:
	void Render();

	void InvokePostponed();

	void SetRunning(bool running);

	virtual void PreRun();
	virtual void PostRun();
	virtual void PostRender();
	virtual void PostObjectsUpdate();
	virtual bool ShouldRender(GameObject* gameObject);
};

