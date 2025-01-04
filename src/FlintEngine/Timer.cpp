#include "Timer.h"

Timer* Timer::main{ nullptr };

Timer* Timer::Main()
{
	return main;
}

double Timer::ClocksToSeconds(clock_t clocks)
{
	return ((double)clocks) / CLOCKS_PER_SEC;
}

Timer::Timer()
	: paused(false)
{
	if (main == nullptr)
	{
		main = this;
	}

	lastFrameTime = clock();
}

Timer::~Timer()
{
	if (main == this)
	{
		main = nullptr;
	}
}

void Timer::FrameStart()
{
	clock_t currentTime = clock();

	clock_t elapsedClocks = currentTime - lastFrameTime;
	if (paused)
	{
		elapsedClocks -= currentTime - pauseTime;
	}

	deltaTime = ClocksToSeconds(elapsedClocks);

	if (deltaTime > s_DeltaTimeToIgnore)
	{
		deltaTime = s_TargetFrameTime;
	}

	lastFrameTime = currentTime;

	invokeQueue.UpdateTimes(deltaTime);
	InvokeTimed();
}

double Timer::GetDeltaTime() const
{
	return deltaTime;
}

void Timer::Pause()
{
	paused = true;
	pauseTime = clock();
}

void Timer::Unpause()
{
	paused = false;
}

double Timer::GetTimeSinceFrameStart() const
{
	const clock_t elapsed_clocks = clock() - lastFrameTime;
	return ClocksToSeconds(elapsed_clocks);
}

void Timer::InvokeOnNextFrame(function<void()> action, double time)
{
	invokeQueue.Add(action, time);
}

void Timer::InvokeTimed()
{
	while (true)
	{
		if (invokeQueue.ClosestTime() > 0.0)
			break;

		function<void()> action = invokeQueue.PopFirst();
		action();
	}
}



void InvokeQueue::Add(std::function<void()> action, double time)
{
	FunctionInvokeArgs args;
	args.fun = action;
	args.time = time;

	// Sortowane wstawianie
	for (vector<FunctionInvokeArgs>::iterator it = items.begin(); it != items.end(); it++)
	{
		if ((*it).time > time)
		{
			items.insert(it, args);
			return;
		}
	}

	items.push_back(args);
}

size_t InvokeQueue::GetSize() const
{
	return items.size();
}

double InvokeQueue::ClosestTime() const
{
	if (items.empty())
		return INFINITY;
	return items[0].time;
}

function<void()> InvokeQueue::PopFirst()
{
	function<void()> result = function<void()>(items[0].fun);

	// Przesuniêcie pozosta³ych
	for (size_t i = 0; i < items.size() - 1; i++)
	{
		items[i] = items[i + 1];
	}
	items.pop_back();

	return result;
}

void InvokeQueue::UpdateTimes(double dTime)
{
	for (size_t i = 0; i < items.size(); i++)
	{
		items[i].time -= dTime;
	}
}
