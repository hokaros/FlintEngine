#pragma once
#include <SDL.h>
#include "ObjectComponent.h"

// Komponent rysuj�cy obiekt
class ObjectRenderer : public ObjectComponent
{
public:
	ObjectRenderer(GameObject& owner);
	void RenderUpdate() override;
	virtual void Render() = 0;
};

