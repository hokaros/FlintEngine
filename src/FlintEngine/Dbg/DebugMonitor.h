#pragma once

#include "DebugGameData.h"

namespace debug
{
	class DebugMonitor
	{
	public:
		void Init(const DebugGameData& debug_data);
		void Render();

	private:
		const DebugGameData* m_DebugData = nullptr;
	};
}