#include "SceneRenderer.h"
#include "../FlatEngine/utility.h"

constexpr Vector s_RenderStart = Vector(0, 0);

SceneRenderer::SceneRenderer(int windowWidth, int windowHeight)
	: m_CurrentViewport(s_RenderStart,
		Vector(windowWidth, windowHeight)
	),
	m_RTSize(windowWidth, windowHeight)
{
}

bool SceneRenderer::Init(SDL_Renderer* renderer, RenderingKey)
{
	m_Renderer = renderer;

	m_OutTexture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_TARGET,
		m_RTSize.x, m_RTSize.y);

	if (!LoadCharsets())
		return false;

	return true;
}

SDL_Renderer* SceneRenderer::GetRenderer()
{
	return m_Renderer;
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
	Rect screen_space_rect = WorldSpaceToScreenSpace(rect);
	RenderTextureScreenSpace(texture, screen_space_rect, angle);
}

void SceneRenderer::RenderRect(const Rect& rect, const Rgb8& color)
{
	SDL_Texture* originalRT = SDL_GetRenderTarget(m_Renderer);
	SDL_SetRenderTarget(m_Renderer, m_OutTexture);

	SDL_Rect ssRect = RectToSDLRect(WorldSpaceToScreenSpace(rect));

	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, 0xFF);

	int result = SDL_RenderFillRect(m_Renderer, &ssRect);
	FE_ASSERT(result == 0, "ERROR: Could not render");

	SDL_SetRenderTarget(m_Renderer, originalRT);
}

void SceneRenderer::RenderLine(const Vector& start, const Vector& end, const Rgb8& color)
{
	SDL_Texture* originalRT = SDL_GetRenderTarget(m_Renderer);
	SDL_SetRenderTarget(m_Renderer, m_OutTexture);

	Vector ssStart = WorldSpaceToScreenSpace(start);
	Vector ssEnd = WorldSpaceToScreenSpace(end);

	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, 0xFF);

	int result = SDL_RenderDrawLine(m_Renderer, ssStart.x, ssStart.y, ssEnd.x, ssEnd.y);
	FE_ASSERT(result == 0, "ERROR: Could not render");

	SDL_SetRenderTarget(m_Renderer, originalRT);
}

void SceneRenderer::RenderWireRect(const Rect& rect, const Rgb8& color)
{
	SDL_Texture* originalRT = SDL_GetRenderTarget(m_Renderer);
	SDL_SetRenderTarget(m_Renderer, m_OutTexture);

	SDL_Rect ssRect = RectToSDLRect(WorldSpaceToScreenSpace(rect));

	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, 0xFF);

	int result = SDL_RenderDrawRect(m_Renderer, &ssRect);
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

void SceneRenderer::RenderTextureScreenSpace(SDL_Texture* texture, const Rect& rect, double angle)
{
	SDL_Texture* originalRT = SDL_GetRenderTarget(m_Renderer);
	SDL_SetRenderTarget(m_Renderer, m_OutTexture); // TODO: don't replace RTs every Render

	FE_ASSERT(m_Renderer != nullptr, "No renderer set");

	SDL_Rect ssRect = RectToSDLRect(rect);

	SDL_Point mid;
	mid.x = ssRect.w / 2;
	mid.y = ssRect.h / 2;
	SDL_RenderCopyEx(m_Renderer, texture, NULL, &(ssRect), angle, &mid, SDL_FLIP_NONE);

	SDL_SetRenderTarget(m_Renderer, originalRT);
}

SceneRenderer::~SceneRenderer()
{
	SDL_DestroyTexture(m_CharsetTex);
	SDL_DestroyTexture(m_OutTexture);
}

Rect SceneRenderer::WorldSpaceToScreenSpace(const Rect& worldSpace) const
{
	return ViewportSpaceToScreenSpace(WorldSpaceToViewportSpace(worldSpace));
}

Vector SceneRenderer::WorldSpaceToScreenSpace(const Vector& worldSpace) const
{
	return ViewportSpaceToScreenSpace(WorldSpaceToViewportSpace(worldSpace));
}

Rect SceneRenderer::WorldSpaceToViewportSpace(const Rect& worldSpace) const
{
	return Rect(WorldSpaceToViewportSpace(worldSpace.pos), worldSpace.size);
}

Vector SceneRenderer::WorldSpaceToViewportSpace(const Vector& worldSpace) const
{
	return worldSpace - m_CurrentViewport.pos;
}

Rect SceneRenderer::ViewportSpaceToScreenSpace(const Rect& viewportSpace) const
{
	Vector screenSpaceSize = Vector(viewportSpace.size.x * m_RTSize.x / m_CurrentViewport.size.x,
		viewportSpace.size.y * m_RTSize.y / m_CurrentViewport.size.y);
	return Rect(ViewportSpaceToScreenSpace(viewportSpace.pos), screenSpaceSize);
}

Vector SceneRenderer::ViewportSpaceToScreenSpace(const Vector& viewportSpace) const
{
	return Vector(viewportSpace.x * m_RTSize.x / m_CurrentViewport.size.x,
		viewportSpace.y * m_RTSize.y / m_CurrentViewport.size.y);
}

bool SceneRenderer::LoadCharsets()
{
	// wczytanie obrazka cs8x8.bmp
	m_Charset = SDL_LoadBMP("Assets/cs8x8.bmp"); // TODO: get from AssetManager
	if (m_Charset == nullptr)
	{
		printf("SDL_LoadBMP(Assets/cs8x8.bmp) error: %s\n", SDL_GetError());
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
