#include "ObjectRenderer.h"


ObjectRenderer::ObjectRenderer(GameObject& owner, SDL_Surface* screen)
	: ObjectComponent(owner), screen(screen) {

}

void ObjectRenderer::RenderUpdate() {
	Render(screen);
}