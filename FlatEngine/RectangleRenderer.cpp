#include "RectangleRenderer.h"

RectangleRenderer::RectangleRenderer(GameObject& owner, SDL_Surface* screen, Uint32 outlineColor, Uint32 fillColor)
	:ObjectRenderer(owner, screen), outlineColor(outlineColor), fillColor(fillColor) {

}

void RectangleRenderer::Render(SDL_Surface* screen) {
	Vector size = gameObject.GetSize();
	DrawRectangle(
		screen,
		gameObject.GetPosition().x,
		gameObject.GetPosition().y,
		size.x,
		size.y,
		outlineColor,
		fillColor
	);
}

ObjectComponent* RectangleRenderer::Copy(GameObject& newOwner) {
	return new RectangleRenderer(newOwner, screen, outlineColor, fillColor);
}