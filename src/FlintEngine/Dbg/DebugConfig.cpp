#include "DebugConfig.h"
#include "imgui/imgui.h"

namespace debug
{
	bool DebugConfig::s_ShouldDebugRenderPhysics = false;
	bool DebugConfig::s_ShouldDebugRenderNavmesh = true;

	bool DebugConfig::ShouldDebugRenderPhysics()
	{
		return s_ShouldDebugRenderPhysics;
	}

	bool DebugConfig::ShouldDebugRenderNavmesh()
	{
		return s_ShouldDebugRenderNavmesh;
	}


	void DebugConfigWindow::Render()
	{
		ImGui::Begin("Debug config");

		ImGui::Checkbox("Physics debug", &DebugConfig::s_ShouldDebugRenderPhysics);
		ImGui::Checkbox("Navmesh debug", &DebugConfig::s_ShouldDebugRenderNavmesh);

		ImGui::End();
	}
}
