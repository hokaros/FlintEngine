#pragma once

namespace debug
{
	enum class DebugMode
	{
		None,
		NavmeshQuery,
		Raycast
	};

	class DebugConfigWindow
	{
	public:
		void Render();
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