#pragma once
#include <ObjectRenderer.h>
#include <ComponentDefinition.h>
#include <SimpleTypes.h>

class TextRenderer
	: public ObjectRenderer
{
	DECLARE_COMPONENT();
public:
	TextRenderer() = default;

	virtual void Render(SceneRenderer& renderer) override;

private:
	uint m_Layer = 1;
	DECLARE_FIELD(m_Layer);

	std::string m_Text = "";
	DECLARE_FIELD(m_Text);

	uint m_FontSize = 10;
	DECLARE_FIELD(m_FontSize);
};