#pragma once
#include "ObjectRenderer.h"
#include "Draw.h"
class RectangleRenderer :
	public ObjectRenderer
{
public:
	RectangleRenderer(GameObject& owner, const Rgb8& color);
	void Render() override;

	ObjectComponent* Copy(GameObject& newOwner) override;
private:
	Rgb8 m_Color;
};

