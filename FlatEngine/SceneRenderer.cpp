#include "SceneRenderer.h"
#include "../FlatEngine/utility.h"

SceneRenderer* SceneRenderer::s_Instance = nullptr;

constexpr Vector s_RenderStart = Vector(0, 0);

SceneRenderer* SceneRenderer::Main()
{
	return s_Instance;
}

SceneRenderer::SceneRenderer(int windowWidth, int windowHeight)
	: m_Width(windowWidth)
	, m_Height(windowHeight)
{
}

bool SceneRenderer::Init(SDL_Renderer* renderer, RenderingKey)
{
	m_Renderer = renderer;

	m_OutTexture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_TARGET,
		m_Width, m_Height);

	if (!LoadCharsets())
		return false;

	if (s_Instance != nullptr)
	{
		FE_WARN("Overwriting SceneRenderer singleton");
	}
	s_Instance = this;
	return true;
}

SDL_Texture* SceneRenderer::GetOutputTexture() const
{
	return m_OutTexture;
}

Rect SceneRenderer::GetRenderedRect() const
{
	return Rect(s_RenderStart, Vector(m_Width, m_Height));
}

void SceneRenderer::RenderTexture(SDL_Texture* texture, const SDL_Rect& rect, double angle)
{
	SDL_Texture* originalRT = SDL_GetRenderTarget(m_Renderer);
	SDL_SetRenderTarget(m_Renderer, m_OutTexture); // TODO: don't replace RTs every Render

	FE_ASSERT(m_Renderer != nullptr, "No renderer set");

	SDL_Point mid;
	mid.x = rect.w / 2;
	mid.y = rect.h / 2;
	SDL_RenderCopyEx(m_Renderer, texture, NULL, &(rect), angle, &mid, SDL_FLIP_NONE);

	SDL_SetRenderTarget(m_Renderer, originalRT);
}

void SceneRenderer::RenderRect(const Rect& rect, const Rgb8& color)
{
	SDL_Texture* originalRT = SDL_GetRenderTarget(m_Renderer);
	SDL_SetRenderTarget(m_Renderer, m_OutTexture);

	SDL_Rect sdl_rect = RectToSDLRect(rect);

	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, 0xFF);

	int result = SDL_RenderFillRect(m_Renderer, &sdl_rect);
	FE_ASSERT(result == 0, "ERROR: Could not render");

	SDL_SetRenderTarget(m_Renderer, originalRT);
}

void SceneRenderer::RenderLine(const Vector& start, const Vector& end, const Rgb8& color)
{
	SDL_Texture* originalRT = SDL_GetRenderTarget(m_Renderer);
	SDL_SetRenderTarget(m_Renderer, m_OutTexture);

	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, 0xFF);

	int result = SDL_RenderDrawLine(m_Renderer, start.x, start.y, end.x, end.y);
	FE_ASSERT(result == 0, "ERROR: Could not render");

	SDL_SetRenderTarget(m_Renderer, originalRT);
}

void SceneRenderer::RenderWireRect(const Rect& rect, const Rgb8& color)
{
	SDL_Texture* originalRT = SDL_GetRenderTarget(m_Renderer);
	SDL_SetRenderTarget(m_Renderer, m_OutTexture);

	SDL_Rect sdl_rect = RectToSDLRect(rect);

	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, 0xFF);

	int result = SDL_RenderDrawRect(m_Renderer, &sdl_rect);
	FE_ASSERT(result == 0, "ERROR: Could not render");

	SDL_SetRenderTarget(m_Renderer, originalRT);
}

void SceneRenderer::DrawStringScreenSpace(int x, int y, const char* text, int fontSize)
{
	SDL_Texture* originalRT = SDL_GetRenderTarget(m_Renderer);
	SDL_SetRenderTarget(m_Renderer, m_OutTexture);

	SDL_Rect src, dest;
	src.w = 8;
	src.h = 8;
	dest.w = fontSize;
	dest.h = fontSize;

	while (*text)
	{
		VectorInt char_coords = GetCharCoordinates(*text);
		src.x = char_coords.x;
		src.y = char_coords.y;

		dest.x = x;
		dest.y = y;
		SDL_RenderCopy(m_Renderer, m_CharsetTex, &src, &dest);
		x += fontSize;
		text++;
	};

	SDL_SetRenderTarget(m_Renderer, originalRT);
}

SceneRenderer::~SceneRenderer()
{
	SDL_DestroyTexture(m_CharsetTex);
	SDL_DestroyTexture(m_OutTexture);
}

bool SceneRenderer::LoadCharsets()
{
	// wczytanie obrazka cs8x8.bmp
	m_Charset = SDL_LoadBMP("resources/cs8x8.bmp");
	if (m_Charset == nullptr)
	{
		printf("SDL_LoadBMP(resources/cs8x8.bmp) error: %s\n", SDL_GetError());
		return false;
	};

	m_CharsetTex = SDL_CreateTextureFromSurface(m_Renderer, m_Charset);
	SDL_SetColorKey(m_Charset, /*enable color key*/true, /*color key*/0x000000);

	return true;
}

VectorInt SceneRenderer::GetCharCoordinates(char c) const
{
	int c_int = c & 255;

	int x = (c_int % 16) * 8;
	int y = (c_int / 16) * 8;

	return VectorInt(x, y);
}
