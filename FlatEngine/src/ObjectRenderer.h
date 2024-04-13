#pragma once
#include <SDL.h>
#include "ObjectComponent.h"

// Komponent rysuj¹cy obiekt
class ObjectRenderer : public ObjectComponent
{
public:
	void RenderUpdate(SceneRenderer& renderer) override;
	virtual void Render(SceneRenderer& renderer) = 0;
};

