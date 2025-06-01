#include "SceneRenderer.h"
#include "utility.h"

constexpr Vector s_RenderStart = Vector(0, 0);

SceneRenderer::SceneRenderer(int windowWidth, int windowHeight)
	: m_CurrentViewport(s_RenderStart,
		Vector(windowWidth, windowHeight)
	)
	, m_RTSize(windowWidth, windowHeight)
	, m_TargetLayers(VectorInt(windowWidth, windowHeight))
{
}

bool SceneRenderer::Init(SDL_Renderer* renderer, RenderingKey)
{
	m_Renderer = renderer;

	m_TargetLayers.Init(m_Renderer);

	if (!LoadCharsets())
		return false;

	return true;
}

SDL_Renderer* SceneRenderer::GetRenderer()
{
	return m_Renderer;
}

SDL_Texture* SceneRenderer::FinalizeFrame()
{
	return m_TargetLayers.MergeLayers();
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

Vector SceneRenderer::GetScreenSize() const
{
	return m_RTSize;
}

void SceneRenderer::RenderTexture(SDL_Texture* texture, const Rect& rect, double angle, rendering::LayerId layer)
{
	Rect screen_space_rect = WorldSpaceToScreenSpace(rect);
	RenderTextureScreenSpace(texture, screen_space_rect, angle, layer);
}

void SceneRenderer::RenderRect(const Rect& rect, const Rgb8& color, rendering::LayerId layer)
{
	RenderTargetScope render_scope = SetTargetLayer(layer);

	SDL_Rect ssRect = RectToSDLRect(WorldSpaceToScreenSpace(rect));

	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, 0xFF);

	int result = SDL_RenderFillRect(m_Renderer, &ssRect);
	FE_ASSERT(result == 0, "ERROR: Could not render");
}

void SceneRenderer::RenderRect(const DirectedRect& rect, const Rgb8& color, rendering::LayerId layer)
{
	RenderTargetScope render_scope = SetTargetLayer(layer);

	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, 0xFF);

	TriangleList triangles;
	RectToTriangles(rect, color, triangles);

	int result = RenderTriangles(triangles);
	FE_ASSERT(result == 0, "ERROR: Could not render");
}

void SceneRenderer::RenderLine(const Vector& start, const Vector& end, const Rgb8& color, rendering::LayerId layer)
{
	RenderTargetScope render_scope = SetTargetLayer(layer);

	Vector ssStart = WorldSpaceToScreenSpace(start);
	Vector ssEnd = WorldSpaceToScreenSpace(end);

	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, 0xFF);

	int result = SDL_RenderDrawLine(m_Renderer, ssStart.x, ssStart.y, ssEnd.x, ssEnd.y);
	FE_ASSERT(result == 0, "ERROR: Could not render");
}

void SceneRenderer::RenderWireRect(const Rect& rect, const Rgb8& color, rendering::LayerId layer)
{
	RenderTargetScope render_scope = SetTargetLayer(layer);

	SDL_Rect ssRect = RectToSDLRect(WorldSpaceToScreenSpace(rect));

	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, 0xFF);

	int result = SDL_RenderDrawRect(m_Renderer, &ssRect);
	FE_ASSERT(result == 0, "ERROR: Could not render");
}

void SceneRenderer::RenderString(const char* text, const Vector& start, int fontSize, rendering::LayerId layer)
{
	const Vector screen_space_start = WorldSpaceToScreenSpace(start);
	const Vector ss_font_size = VectorWorldSpaceToScreenSpace(Vector(fontSize, fontSize));
	DrawStringScreenSpace(screen_space_start.x, screen_space_start.y, text, ss_font_size.y, layer);
}

void SceneRenderer::Clear(const Rgb8& clear_color)
{
	m_TargetLayers.ClearLayers();
	RenderRect(GetRenderedRect(), clear_color, rendering::LayerId(0)); // Render on first layer
}

void SceneRenderer::DrawStringScreenSpace(int x, int y, const char* text, int fontSize, rendering::LayerId layer)
{
	RenderTargetScope render_scope = SetTargetLayer(layer);

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
}

void SceneRenderer::RenderTextureScreenSpace(SDL_Texture* texture, const Rect& rect, double angle, rendering::LayerId layer)
{
	RenderTargetScope render_scope = SetTargetLayer(layer);

	FE_ASSERT(m_Renderer != nullptr, "No renderer set");

	SDL_Rect ssRect = RectToSDLRect(rect);

	SDL_Point mid;
	mid.x = ssRect.w / 2;
	mid.y = ssRect.h / 2;
	SDL_RenderCopyEx(m_Renderer, texture, NULL, &(ssRect), angle, &mid, SDL_FLIP_NONE);
}

SceneRenderer::~SceneRenderer()
{
	SDL_DestroyTexture(m_CharsetTex);
}

Rect SceneRenderer::WorldSpaceToScreenSpace(const Rect& worldSpace) const
{
	return ViewportSpaceToScreenSpace(WorldSpaceToViewportSpace(worldSpace));
}

Vector SceneRenderer::WorldSpaceToScreenSpace(const Vector& worldSpace) const
{
	return ViewportSpaceToScreenSpace(WorldSpaceToViewportSpace(worldSpace));
}

Vector SceneRenderer::ScreenSpaceToWorldSpace(const Vector& screenSpace) const
{
	return ViewportSpaceToWorldSpace(ScreenSpaceToViewportSpace(screenSpace));
}

Vector SceneRenderer::VectorWorldSpaceToScreenSpace(const Vector& worldSpace) const
{
	return VectorViewportSpaceToScreenSpace(VectorWorldSpaceToViewportSpace(worldSpace));
}

SDL_Texture* SceneRenderer::CreateTextureFromSurface(SDL_Surface* surface)
{
	return SDL_CreateTextureFromSurface(m_Renderer, surface);
}

Rect SceneRenderer::WorldSpaceToViewportSpace(const Rect& worldSpace) const
{
	return Rect(WorldSpaceToViewportSpace(worldSpace.pos), VectorWorldSpaceToViewportSpace(worldSpace.size));
}

Vector SceneRenderer::WorldSpaceToViewportSpace(const Vector& worldSpace) const
{
	return worldSpace - m_CurrentViewport.pos;
}

Vector SceneRenderer::ViewportSpaceToWorldSpace(const Vector& viewportSpace) const
{
	return viewportSpace + m_CurrentViewport.pos;
}

Vector SceneRenderer::VectorWorldSpaceToViewportSpace(const Vector& world_space) const
{
	return world_space;
}

Rect SceneRenderer::ViewportSpaceToScreenSpace(const Rect& viewportSpace) const
{
	Vector screenSpacePos = ViewportSpaceToScreenSpace(viewportSpace.pos);
	Vector screenSpaceSize = VectorViewportSpaceToScreenSpace(viewportSpace.size);
	return Rect(screenSpacePos, screenSpaceSize);
}

Vector SceneRenderer::ViewportSpaceToScreenSpace(const Vector& viewportSpace) const
{
	float x = viewportSpace.x * m_RTSize.x / m_CurrentViewport.size.x;
	float y = viewportSpace.y * m_RTSize.y / m_CurrentViewport.size.y;
	return Vector(x, y);
}

Vector SceneRenderer::ScreenSpaceToViewportSpace(const Vector& screenSpace) const
{
	float x = screenSpace.x * m_CurrentViewport.size.x / m_RTSize.x;
	float y = screenSpace.y * m_CurrentViewport.size.y / m_RTSize.y;
	return Vector(x, y);
}

Vector SceneRenderer::VectorViewportSpaceToScreenSpace(const Vector& viewport_space) const
{
	return ViewportSpaceToScreenSpace(viewport_space); // Vector as a point
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

int SceneRenderer::RenderTriangles(const TriangleList& triangles)
{
	std::vector<SDL_Vertex> vertices;
	VerticesToSDLVertices(triangles.vertices, vertices);

	return SDL_RenderGeometry(m_Renderer, nullptr, vertices.data(), vertices.size(), triangles.indices.data(), triangles.indices.size());
}

RenderTargetScope SceneRenderer::SetTargetLayer(rendering::LayerId layer_id)
{
	return RenderTargetScope(m_Renderer, m_TargetLayers.GetOrCreateLayer(layer_id));
}

void SceneRenderer::RectToTriangles(const DirectedRect& rect, const Rgb8& color, TriangleList& triangles)
{
	triangles.vertices.push_back({ rect.GetCorner1(), color });
	triangles.vertices.push_back({ rect.GetCorner2(), color });
	triangles.vertices.push_back({ rect.GetCorner3(), color });
	triangles.vertices.push_back({ rect.GetCorner4(), color });
	
	triangles.indices.push_back(0);
	triangles.indices.push_back(1);
	triangles.indices.push_back(2);
	triangles.indices.push_back(1);
	triangles.indices.push_back(2);
	triangles.indices.push_back(3);
}

void SceneRenderer::VerticesToSDLVertices(const std::vector<Vertex>& vertices, std::vector<SDL_Vertex>& sdl_vertices)
{
	for (const Vertex& in_v : vertices)
	{
		SDL_Vertex sdl_v;
		sdl_v.position = VectorToSDLPoint(in_v.pos);
		sdl_v.color = Rgb8ToSDLColor(in_v.color);

		sdl_vertices.push_back(sdl_v);
	}
}

SDL_FPoint SceneRenderer::VectorToSDLPoint(const Vector& v)
{
	SDL_FPoint p;
	p.x = v.x;
	p.y = v.y;
	return p;
}

SDL_Color SceneRenderer::Rgb8ToSDLColor(const Rgb8& color)
{
	SDL_Color out_c;
	out_c.r = color.r;
	out_c.g = color.g;
	out_c.b = color.b;
	out_c.a = 0xFF;
	return out_c;
}
