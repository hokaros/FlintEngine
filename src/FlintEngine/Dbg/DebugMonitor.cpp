#include "DebugMonitor.h"

#include <utility.h>
#include "imgui/imgui.h"

namespace debug
{
	void DebugMonitor::Init(const DebugGameData& debug_data)
	{
		m_DebugData = &debug_data;
	}

	void DebugMonitor::Render()
	{
		FE_ASSERT(m_DebugData != nullptr, "No debug data passed. Has Init() been called?");

		ImGui::Begin("Debug monitor");

		ImGui::Text("FPS: %0.2f", m_DebugData->GetFrameRate());

		ImGui::End();
	}
}
