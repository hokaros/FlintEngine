#pragma once
#include <ComponentDefinition.h>
#include <ObjectRenderer.h>
#include <Rendering/SceneRenderer.h>

class PlayerLabel
	: public ObjectRenderer
{
	DECLARE_COMPONENT();
public:
	virtual void Render(SceneRenderer& renderer) override;
	
private:
	uint m_Layer = 1;
	DECLARE_FIELD(m_Layer);

	std::string m_Text = "";
	DECLARE_FIELD(m_Text);

	uint m_FontSize = 10;
	DECLARE_FIELD(m_FontSize);

	Vector m_Offset;
	DECLARE_FIELD(m_Offset);
};