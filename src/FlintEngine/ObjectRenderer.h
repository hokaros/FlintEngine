#pragma once
#include <SDL.h>
#include <Core/ObjectComponent.h>

class SceneRenderer;

// Komponent rysuj�cy obiekt
class ObjectRenderer : public ObjectComponent
{
public:
	// TODO: we don't need this
	void RenderUpdate(SceneRenderer& renderer) override final;
	virtual void Render(SceneRenderer& renderer) = 0;
};

