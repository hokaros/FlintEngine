#include "TextRenderer.h"
#include <Core/GameObject.h>

DEFINE_COMPONENT(TextRenderer);

DEFINE_FIELD(TextRenderer, m_Layer);
DEFINE_FIELD(TextRenderer, m_Text);
DEFINE_FIELD(TextRenderer, m_FontSize);

void TextRenderer::Render(SceneRenderer& renderer)
{
	const Vector& self_pos = GetOwner().GetPosition();
	renderer.RenderString(m_Text.c_str(), self_pos, m_FontSize, m_Layer);
}
