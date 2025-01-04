#include "DebugGameData.h"

#include <Timer.h>

namespace debug
{
	float DebugGameData::GetFrameRate() const
	{
		return m_FrameRateTracker.GetFrameRate();
	}

	void DebugGameData::PostFrame()
	{
		m_FrameRateTracker.PostFrame();
	}

	float FrameRateTracker::GetFrameRate() const
	{
		return m_FrameRate;
	}

	void FrameRateTracker::PostFrame()
	{
		m_FramesSinceLastFrameRateProbe++;

		if (m_FramesSinceLastFrameRateProbe >= s_NumProbedFrames)
		{
			Probe();

			m_FramesSinceLastFrameRateProbe = 0;
		}
	}

	void FrameRateTracker::Probe()
	{
		const clock_t new_clock = clock();

		if (m_PrevProbeClock)
		{
			const float time_elapsed = Timer::ClocksToSeconds(new_clock - *m_PrevProbeClock);

			m_FrameRate = s_NumProbedFrames / time_elapsed;
		}

		m_PrevProbeClock = new_clock;
	}
}