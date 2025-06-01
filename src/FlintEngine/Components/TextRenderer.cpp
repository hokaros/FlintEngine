#include "TextRenderer.h"
#include <Core/GameObject.h>
#include <Rendering/SceneRenderer.h>

DEFINE_COMPONENT(TextRenderer);

DEFINE_FIELD(TextRenderer, m_Layer);
DEFINE_FIELD(TextRenderer, m_Text);
DEFINE_FIELD(TextRenderer, m_FontSize);

void TextRenderer::Render(SceneRenderer& renderer)
{
	const Vector& self_pos = GetOwner().GetWorldPosition();
	renderer.RenderString(m_Text.c_str(), self_pos, m_FontSize, rendering::LayerId(m_Layer));
}
