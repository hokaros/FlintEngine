#pragma once
#include <time.h>
#include <functional>
#include <vector>

using std::pair;
using std::function;
using std::vector;

struct FunctionInvokeArgs {
	function<void()> fun;
	double time;
};

class InvokeQueue {
public:
	void Add(function<void()> action, double time);

	size_t GetSize() const;
	double ClosestTime() const;
	function<void()> PopFirst();

	void UpdateTimes(double dTime);
private:
	vector<FunctionInvokeArgs> items;
};

class Timer
{
public:
	Timer();
	~Timer();

	void FrameStart();
	// Zwraca czas od ostatniej klatki [s]
	double GetDeltaTime() const;
	// Zatrzymuje up³yw czasu
	void Pause();
	// Odblokowuje up³yw czasu
	void Unpause();

	double GetTimeSinceFrameStart() const;

	// Umo¿liwia wykonanie funkcji po pewnym czasie
	void InvokeOnNextFrame(function<void()> action, double time);
	// Wykonuje funkcje, których czas nadszed³
	void InvokeTimed();

	static Timer* Main();

	static double ClocksToSeconds(clock_t clocks);

private:
	clock_t lastFrameTime = 0;
	double deltaTime = 0.0;

	bool paused = false;
	clock_t pauseTime = 0;

	InvokeQueue invokeQueue;

	static Timer* main;
	static constexpr double s_DeltaTimeToIgnore = 1.0; // dt powy¿ej tego czasu zostaje zignorowany i uciêty do docelowego czasu ramki
	static constexpr double s_TargetFrameTime = 1 / 60.0;
};

