#include "DebugOverlayText.h"

namespace debug
{
	void DebugOverlayText::UpdateCornerText(const std::string& text)
	{
		m_CornerText = text;
	}

	void DebugOverlayText::PreUpdate()
	{
		m_CornerText = "";
	}

	void DebugOverlayText::Render(SceneRenderer& renderer)
	{
		constexpr int font_size = 14;
		const int viewport_lower_bound = renderer.GetViewport().size.y;
		renderer.DrawStringScreenSpace(0, viewport_lower_bound - font_size, m_CornerText.c_str(), font_size, rendering::LayerId::DEBUG_SCREENSPACE);
	}
}