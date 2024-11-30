#pragma once

namespace debug
{
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

	private:
		static bool s_ShouldDebugRenderPhysics;
		static bool s_ShouldDebugRenderNavmesh;

		friend DebugConfigWindow;
	};
}