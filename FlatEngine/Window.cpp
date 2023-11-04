#include "Window.h"

Window* Window::s_MainWindow = nullptr;

Window::Window(int width, int height)
	: m_Width(width)
	, m_Height(height) 
{
	if (s_MainWindow == nullptr) 
	{
		s_MainWindow = this;
	}
}

Window::~Window() 
{
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

Window* Window::Main() 
{
	return s_MainWindow;
}

bool Window::Init() 
{
	int rc;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
	{
		printf("SDL_Init error: %s\n", SDL_GetError());
		return false;
	}

	// tryb pe³noekranowy / fullscreen mode
//	rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
//	                                 &window, &renderer);
	rc = SDL_CreateWindowAndRenderer(m_Width, m_Height, 0,
		&m_Window, &m_Renderer);
	if (rc != 0) 
	{
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return false;
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(m_Renderer, m_Width, m_Height);

	SDL_SetWindowTitle(m_Window, "Labyrinth Shooter");

	m_Scrtex = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		m_Width, m_Height);

	m_DebugRenderer = new DebugRenderer(m_Renderer);

	if (!LoadCharsets())
		return false;

	return true;
}

void Window::Present() 
{
	SDL_RenderPresent(m_Renderer);

	SDL_SetRenderDrawColor(m_Renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(m_Renderer);
}

void Window::RenderTexture(SDL_Texture* texture, const SDL_Rect& rect, double angle) 
{
	SDL_Point mid;
	mid.x = rect.w / 2;
	mid.y = rect.h / 2;
	SDL_RenderCopyEx(m_Renderer, texture, NULL, &(rect), angle, &mid, SDL_FLIP_NONE);
}

void Window::DrawString(int x, int y, const char* text, int fontSize) 
{
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = fontSize;
	d.h = fontSize;

	while (*text) 
	{
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_RenderCopy(m_Renderer, m_CharsetTex, &s, &d);
		x += fontSize;
		text++;
	};
}

SDL_Renderer* Window::GetRenderer() const 
{
	return m_Renderer;
}

int Window::GetWidth() const 
{
	return m_Width;
}

int Window::GetHeight() const 
{
	return m_Height;
}

bool Window::LoadCharsets() 
{
	// wczytanie obrazka cs8x8.bmp
	m_Charset = SDL_LoadBMP("resources/cs8x8.bmp");
	if (m_Charset == nullptr) 
	{
		printf("SDL_LoadBMP(resources/cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_DestroyTexture(m_Scrtex);
		SDL_DestroyWindow(m_Window);
		SDL_DestroyRenderer(m_Renderer);
		SDL_Quit();
		return false;
	};

	m_CharsetTex = SDL_CreateTextureFromSurface(m_Renderer, m_Charset);
	SDL_SetColorKey(m_Charset, /*enable color key*/true, /*color key*/0x000000);

	return true;
}
