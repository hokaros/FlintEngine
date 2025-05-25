#pragma once
#include <vector>

namespace debug
{
	enum class DebugMode
	{
		None,
		NavmeshQuery,
		NavmeshLineContainment,
		NavmeshDirectedRectContainment,
		Raycast
	};

	const char* DebugModeToString(DebugMode mode);

	class DebugConfigWindow
	{
	public:
		void Render();

	private:
		struct DebugModeEntry
		{
			const char* display_name;
			DebugMode mode;

			constexpr DebugModeEntry(const char* display_name, DebugMode mode)
				: display_name(display_name)
				, mode(mode)
			{}
		};

	private:
		void GatherDebugModes(std::vector<DebugModeEntry>& out_modes);
		void GatherDebugMode(DebugMode mode, std::vector<DebugModeEntry>& out_modes);

	private:
		std::vector<DebugModeEntry> m_DebugModes;
	};

	class DebugConfig
	{
	public:
		static bool ShouldDebugRenderPhysics();
		static bool ShouldDebugRenderNavmesh();
		static DebugMode GetDebugMode();

	private:
		static bool s_ShouldDebugRenderPhysics;
		static bool s_ShouldDebugRenderNavmesh;
		static DebugMode s_DebugMode;

		friend DebugConfigWindow;
	};
}