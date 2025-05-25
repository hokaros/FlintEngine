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

		if (m_DebugModes.empty())
		{
			GatherDebugModes(m_DebugModes);
		}

		if (ImGui::BeginCombo("Debug mode", DebugModeToString(DebugConfig::s_DebugMode), ImGuiComboFlags_None))
		{
			for (const DebugModeEntry& debug_mode : m_DebugModes)
			{
				const bool is_selected = DebugConfig::s_DebugMode == debug_mode.mode;
				if (ImGui::Selectable(debug_mode.display_name, is_selected))
				{
					DebugConfig::s_DebugMode = debug_mode.mode;
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		ImGui::End();
	}

	void DebugConfigWindow::GatherDebugModes(std::vector<DebugModeEntry>& out_modes)
	{
		GatherDebugMode(DebugMode::None, out_modes);
		GatherDebugMode(DebugMode::NavmeshQuery, out_modes);
		GatherDebugMode(DebugMode::NavmeshLineContainment, out_modes);
		GatherDebugMode(DebugMode::NavmeshDirectedRectContainment, out_modes);
		GatherDebugMode(DebugMode::Raycast, out_modes);
	}

	void DebugConfigWindow::GatherDebugMode(DebugMode mode, std::vector<DebugModeEntry>& out_modes)
	{
		out_modes.emplace_back(DebugModeToString(mode), mode);
	}

	const char* DebugModeToString(DebugMode mode)
	{
		switch (mode)
		{
		case debug::DebugMode::None:
			return "None";

		case debug::DebugMode::NavmeshQuery:
			return "NavmeshQuery";

		case debug::DebugMode::NavmeshLineContainment:
			return "NavmeshLineContainment";

		case debug::DebugMode::NavmeshDirectedRectContainment:
			return "NavmeshDirectedRectContainment";

		case debug::DebugMode::Raycast:
			return "Raycast";

		default:
			return "?";
		}
	}
}
