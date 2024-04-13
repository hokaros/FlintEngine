#include "DebugConfig.h"
#include "imgui/imgui.h"

namespace debug
{
	bool DebugConfig::s_ShouldDebugRenderPhysics = false;

	bool DebugConfig::ShouldDebugRenderPhysics()
	{
		return s_ShouldDebugRenderPhysics;
	}


	void DebugConfigWindow::Render()
	{
		ImGui::Begin("Debug config");

		ImGui::Checkbox("Physics debug", &DebugConfig::s_ShouldDebugRenderPhysics);

		ImGui::End();
	}
}
