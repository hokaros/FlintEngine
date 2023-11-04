#include "Window.h"

Window* Window::s_MainWindow = nullptr;

Window::Window(int width, int height)
	: m_Width(width), m_Height(height) {
	if (s_MainWindow == NULL) {
		s_MainWindow = this;
	}
}

Window::~Window() {
	if (m_Window != nullptr) {
		SDL_DestroyWindow(m_Window);
	}
	SDL_Quit();

	if (s_MainWindow == this) {
		s_MainWindow = nullptr;
	}

	if (m_DebugRenderer != nullptr)
	{
		delete m_DebugRenderer;
	}
}

Window* Window::Main() {
	return s_MainWindow;
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
	rc = SDL_CreateWindowAndRenderer(m_Width, m_Height, 0,
		&m_Window, &m_Renderer);
	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return false;
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(m_Renderer, m_Width, m_Height);
	SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(m_Window, "Labyrinth Shooter");

	m_Screen = SDL_CreateRGBSurface(0, m_Width, m_Height, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	m_Scrtex = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		m_Width, m_Height);

	m_DebugRenderer = new DebugRenderer(m_Screen);

	if (!LoadCharsets())
		return false;

	return true;
}

void Window::Render() {
	// Renderowanie
	SDL_UpdateTexture(m_Scrtex, /* rect = */nullptr, m_Screen->pixels, m_Screen->pitch);
	//SDL_RenderClear(renderer);
	SDL_RenderCopy(m_Renderer, m_Scrtex, nullptr, nullptr);

	RenderRequestedTextures();

	SDL_RenderPresent(m_Renderer);
}

void Window::RenderTexture(SDL_Texture* texture, const SDL_Rect& rect, double angle) {
	m_RenderTextures.push_back(
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
		SDL_BlitScaled(m_Charset, &s, m_Screen, &d);
		//SDL_BlitSurface(charset, &s, screen, &d);
		x += fontSize;
		text++;
	};
}

SDL_Surface* Window::GetScreen() const {
	return m_Screen;
}

SDL_Renderer* Window::GetRenderer() const {
	return m_Renderer;
}

int Window::GetWidth() const {
	return m_Width;
}

int Window::GetHeight() const {
	return m_Height;
}

bool Window::LoadCharsets() {
	// wczytanie obrazka cs8x8.bmp
	m_Charset = SDL_LoadBMP("resources/cs8x8.bmp");
	if (m_Charset == nullptr) {
		printf("SDL_LoadBMP(resources/cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(m_Screen);
		SDL_DestroyTexture(m_Scrtex);
		SDL_DestroyWindow(m_Window);
		SDL_DestroyRenderer(m_Renderer);
		SDL_Quit();
		return false;
	};

	SDL_SetColorKey(m_Charset, /*enable color key*/true, /*color key*/0x000000);

	return true;
}

void Window::RenderRequestedTextures()
{
	for (TextureRenderArgs tra : m_RenderTextures) {
		SDL_Point mid;
		mid.x = tra.rect.w / 2;
		mid.y = tra.rect.h / 2;
		SDL_RenderCopyEx(m_Renderer, tra.texture, NULL, &(tra.rect), tra.angle, &mid, SDL_FLIP_NONE);
	}
	m_RenderTextures.clear();
}
