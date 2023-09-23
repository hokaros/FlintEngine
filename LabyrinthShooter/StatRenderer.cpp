#include "StatRenderer.h"

void StatRenderer::UpdateStat(int newStat) {
	stat = newStat;
}


BMPStats::BMPStats(SDL_Surface* bitmap, const VectorInt& elementSize, const VectorInt& startPosition)
	: bitmap(bitmap), elementSize(elementSize), startPos(startPosition) {

}

void BMPStats::Render() {
	// Wyœwietl tyle elementów, ile wynosi stat
	SDL_Surface* screen = Window::Main()->GetScreen();

	SDL_Rect dest;
	dest.x = startPos.x;
	dest.y = startPos.y;
	dest.w = elementSize.x;
	dest.h = elementSize.y;

	for (int i = 0; i < stat; i++) {
		SDL_BlitScaled(bitmap, NULL, screen, &dest);

		dest.x += elementSize.x;
	}
}