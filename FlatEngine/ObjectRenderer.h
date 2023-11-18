#pragma once
#include <SDL.h>
#include "ObjectComponent.h"

// Komponent rysuj¹cy obiekt
class ObjectRenderer : public ObjectComponent
{
public:
	void RenderUpdate() override;
	virtual void Render() = 0;
};

