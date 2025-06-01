#pragma once
#include <iostream>
#include <Rendering/SceneRenderer.h>

namespace debug
{
	class DebugOverlayText
	{
	public:
		// Text persisting for one Update cycle
		void UpdateCornerText(const std::string& text);

		void PreUpdate();
		void Render(SceneRenderer& renderer);

	private:
		std::string m_CornerText;
	};
}