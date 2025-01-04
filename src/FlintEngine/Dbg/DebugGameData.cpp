#include "DebugGameData.h"

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
			const float time_elapsed = GetSecondsFromClocks(new_clock - *m_PrevProbeClock);

			m_FrameRate = s_NumProbedFrames / time_elapsed;
		}

		m_PrevProbeClock = new_clock;
	}

	float FrameRateTracker::GetSecondsFromClocks(clock_t clocks) const
	{
		return (float)clocks / CLOCKS_PER_SEC;
	}
}