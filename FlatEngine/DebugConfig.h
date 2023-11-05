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

	private:
		static bool s_ShouldDebugRenderPhysics;

		friend DebugConfigWindow;
	};
}