#include "PlayerLabel.h"
#include <Core/GameObject.h>

DEFINE_COMPONENT(PlayerLabel);

DEFINE_FIELD(PlayerLabel, m_Layer);
DEFINE_FIELD(PlayerLabel, m_Text);
DEFINE_FIELD(PlayerLabel, m_FontSize);
DEFINE_FIELD(PlayerLabel, m_Offset);


void PlayerLabel::Render(SceneRenderer& renderer)
{
	const Vector& render_pos = GetOwner().GetWorldPosition() + m_Offset;
	renderer.RenderString(m_Text.c_str(), render_pos, m_FontSize, m_Layer);
}
