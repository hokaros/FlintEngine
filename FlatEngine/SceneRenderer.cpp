#include "SceneRenderer.h"
#include "../FlatEngine/utility.h"

SceneRenderer* SceneRenderer::s_Instance = nullptr;

constexpr Vector s_RenderStart = Vector(0, 0);

SceneRenderer* SceneRenderer::Main()
{
	return s_Instance;
}

SceneRenderer::SceneRenderer(int windowWidth, int windowHeight)
	: m_CurrentViewport(s_RenderStart,
		Vector(windowWidth, windowHeight)
	)
{
}

bool SceneRenderer::Init(SDL_Renderer* renderer, RenderingKey)
{
	m_Renderer = renderer;

	m_OutTexture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_TARGET,
		m_CurrentViewport.size.x, m_CurrentViewport.size.y);

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

void SceneRenderer::SetViewport(const Rect& viewport)
{
	m_CurrentViewport = viewport;
}

const Rect& SceneRenderer::GetRenderedRect() const
{
	return m_CurrentViewport;
}

Rect& SceneRenderer::GetViewport()
{
	return m_CurrentViewport;
}

void SceneRenderer::RenderTexture(SDL_Texture* texture, const Rect& rect, double angle)
{
	SDL_Texture* originalRT = SDL_GetRenderTarget(m_Renderer);
	SDL_SetRenderTarget(m_Renderer, m_OutTexture); // TODO: don't replace RTs every Render

	FE_ASSERT(m_Renderer != nullptr, "No renderer set");

	SDL_Rect vsRect = RectToSDLRect(GetRectViewportSpace(rect));

	SDL_Point mid;
	mid.x = vsRect.w / 2;
	mid.y = vsRect.h / 2;
	SDL_RenderCopyEx(m_Renderer, texture, NULL, &(vsRect), angle, &mid, SDL_FLIP_NONE);

	SDL_SetRenderTarget(m_Renderer, originalRT);
}

void SceneRenderer::RenderRect(const Rect& rect, const Rgb8& color)
{
	SDL_Texture* originalRT = SDL_GetRenderTarget(m_Renderer);
	SDL_SetRenderTarget(m_Renderer, m_OutTexture);

	SDL_Rect vsRect = RectToSDLRect(GetRectViewportSpace(rect));

	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, 0xFF);

	int result = SDL_RenderFillRect(m_Renderer, &vsRect);
	FE_ASSERT(result == 0, "ERROR: Could not render");

	SDL_SetRenderTarget(m_Renderer, originalRT);
}

void SceneRenderer::RenderLine(const Vector& start, const Vector& end, const Rgb8& color)
{
	SDL_Texture* originalRT = SDL_GetRenderTarget(m_Renderer);
	SDL_SetRenderTarget(m_Renderer, m_OutTexture);

	Vector vsStart = GetPointViewportSpace(start);
	Vector vsEnd = GetPointViewportSpace(end);

	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, 0xFF);

	int result = SDL_RenderDrawLine(m_Renderer, vsStart.x, vsStart.y, vsEnd.x, vsEnd.y);
	FE_ASSERT(result == 0, "ERROR: Could not render");

	SDL_SetRenderTarget(m_Renderer, originalRT);
}

void SceneRenderer::RenderWireRect(const Rect& rect, const Rgb8& color)
{
	SDL_Texture* originalRT = SDL_GetRenderTarget(m_Renderer);
	SDL_SetRenderTarget(m_Renderer, m_OutTexture);

	SDL_Rect vsRect = RectToSDLRect(GetRectViewportSpace(rect));

	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, 0xFF);

	int result = SDL_RenderDrawRect(m_Renderer, &vsRect);
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

Rect SceneRenderer::GetRectViewportSpace(const Rect& worldSpace) const
{
	return Rect(GetPointViewportSpace(worldSpace.pos), worldSpace.size);
}

Vector SceneRenderer::GetPointViewportSpace(const Vector& worldSpace) const
{
	return worldSpace - m_CurrentViewport.pos;
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
