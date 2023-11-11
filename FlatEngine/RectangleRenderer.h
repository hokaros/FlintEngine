#pragma once
#include "ObjectRenderer.h"
#include "ComponentDefinition.h"
#include "Draw.h"
class RectangleRenderer :
	public ObjectRenderer
{
	DECLARE_COMPONENT();

public:
	RectangleRenderer(GameObject& owner);
	RectangleRenderer(GameObject& owner, const Rgb8& color);
	void Render() override;

	ObjectComponent* Copy(GameObject& newOwner) override;
private:
	Rgb8 m_Color;

	DECLARE_FIELD(m_Color);
};

