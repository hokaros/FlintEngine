#pragma once
#include <iostream>

#include <optional>

namespace debug
{
	class FrameRateTracker
	{
	public:
		float GetFrameRate() const;

		void PostFrame();

	private:
		void Probe();
		float GetSecondsFromClocks(clock_t clocks) const;

	private:
		float m_FrameRate = 0.f;

		std::optional<clock_t> m_PrevProbeClock = std::nullopt;
		size_t m_FramesSinceLastFrameRateProbe = 0;

		static constexpr size_t s_NumProbedFrames = 60;
	};

	class DebugGameData
	{
	public:
		float GetFrameRate() const;

		void PostFrame();

	private:
		FrameRateTracker m_FrameRateTracker;
	};
}