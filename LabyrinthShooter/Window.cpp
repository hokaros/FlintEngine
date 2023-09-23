#include "Window.h"

Window* Window::mainWindow{ NULL };

Window::Window(int width, int height)
	: width(width), height(height) {
	if (mainWindow == NULL) {
		mainWindow = this;
	}
}

Window::~Window() {
	if (window != NULL) {
		SDL_DestroyWindow(window);
	}
	SDL_Quit();

	if (mainWindow == this) {
		mainWindow = NULL;
	}
}

Window* Window::Main() {
	return mainWindow;
}

bool Window::Init() {
	int rc;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return false;
	}

	// tryb pe³noekranowy / fullscreen mode
//	rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
//	                                 &window, &renderer);
	rc = SDL_CreateWindowAndRenderer(width, height, 0,
		&window, &renderer);
	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return false;
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, width, height);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "Labyrinth Shooter");

	screen = SDL_CreateRGBSurface(0, width, height, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		width, height);

	if (!LoadCharsets())
		return false;

	return true;
}

void Window::Render() {
	// Renderowanie
	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	//SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);

	for (TextureRenderArgs tra : renderTextures) {
		SDL_Point mid;
		mid.x = tra.rect.w / 2;
		mid.y = tra.rect.h / 2;
		SDL_RenderCopyEx(renderer, tra.texture, NULL, &(tra.rect), tra.angle, &mid, SDL_FLIP_NONE);
	}
	renderTextures.clear();

	SDL_RenderPresent(renderer);
}

void Window::RenderTexture(SDL_Texture* texture, const SDL_Rect& rect, double angle) {
	renderTextures.push_back(
		TextureRenderArgs(texture, rect, angle)
	);
}

void Window::DrawString(int x, int y, const char* text, int fontSize) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = fontSize;
	d.h = fontSize;

	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitScaled(charset, &s, screen, &d);
		//SDL_BlitSurface(charset, &s, screen, &d);
		x += fontSize;
		text++;
	};
}

SDL_Surface* Window::GetScreen() const {
	return screen;
}

SDL_Renderer* Window::GetRenderer() const {
	return renderer;
}

int Window::GetWidth() const {
	return width;
}

int Window::GetHeight() const {
	return height;
}

bool Window::LoadCharsets() {
	// wczytanie obrazka cs8x8.bmp
	charset = SDL_LoadBMP("resources/cs8x8.bmp");
	if (charset == NULL) {
		printf("SDL_LoadBMP(resources/cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return false;
	};

	SDL_SetColorKey(charset, true, 0x000000);

	return true;
}