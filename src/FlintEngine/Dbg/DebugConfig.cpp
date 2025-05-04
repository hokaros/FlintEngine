#include "DebugConfig.h"
#include "imgui/imgui.h"

namespace debug
{
	bool DebugConfig::s_ShouldDebugRenderPhysics = false;
	bool DebugConfig::s_ShouldDebugRenderNavmesh = true;
	DebugMode DebugConfig::s_DebugMode = DebugMode::NavmeshQuery;

	bool DebugConfig::ShouldDebugRenderPhysics()
	{
		return s_ShouldDebugRenderPhysics;
	}

	bool DebugConfig::ShouldDebugRenderNavmesh()
	{
		return s_ShouldDebugRenderNavmesh;
	}

	DebugMode DebugConfig::GetDebugMode()
	{
		return s_DebugMode;
	}


	void DebugConfigWindow::Render()
	{
		ImGui::Begin("Debug config");

		ImGui::Checkbox("Physics debug", &DebugConfig::s_ShouldDebugRenderPhysics);
		ImGui::Checkbox("Navmesh debug", &DebugConfig::s_ShouldDebugRenderNavmesh);

		ImGui::End();
	}
}
