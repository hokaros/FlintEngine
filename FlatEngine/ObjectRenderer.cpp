#include "ObjectRenderer.h"


ObjectRenderer::ObjectRenderer(GameObject& owner)
	: ObjectComponent(owner) {

}

void ObjectRenderer::RenderUpdate() {
	Render();
}