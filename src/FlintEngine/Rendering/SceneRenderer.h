#pragma once
#include <Rendering/TargetLayersContainer.h>
#include <Rendering/LayerId.h>

#include "Draw.h"
#include <Math/DirectedRect.h>

class Window;
class SceneEditor;
class EditorGameRunner;

class RenderingKey
{
private:
	RenderingKey() = default;
	
	friend class Window;
	friend class SceneEditor;
	friend class EditorGameRunner;
};

class SceneRenderer
{
public:
	SceneRenderer(int windowWidth, int windowHeight); // TODO: don't require windowWidth and windowHeight here, just set the render target for the SceneRenderer before any rendering
	bool Init(SDL_Renderer* renderer, RenderingKey);

	SDL_Renderer* GetRenderer();

	SDL_Texture* FinalizeFrame();

	void RenderTexture(SDL_Texture* texture, const Rect& rect, double angle, rendering::LayerId layer);
	void RenderRect(const Rect& rect, const Rgb8& color, rendering::LayerId layer);
	void RenderRect(const DirectedRect& rect, const Rgb8& color, rendering::LayerId layer);
	void RenderLine(const Vector& start, const Vector& end, const Rgb8& color, rendering::LayerId layer);
	void RenderWireRect(const Rect& rect, const Rgb8& color, rendering::LayerId layer);

	void RenderString(const char* text, const Vector& start, int fontSize, rendering::LayerId layer); // Renders string in world space. start is the upper left corner

	void Clear(const Rgb8& clear_color);

	void SetViewport(const Rect& viewport);
	const Rect& GetRenderedRect() const;
	Rect& GetViewport();

	Vector GetScreenSize() const;

	// narysowanie napisu txt na ekranie, zaczynaj¹c od punktu (x, y)
	void DrawStringScreenSpace(int x, int y, const char* text, int fontSize, rendering::LayerId layer);
	void RenderTextureScreenSpace(SDL_Texture* texture, const Rect& rect, double angle, rendering::LayerId layer);

	[[nodiscard]] Rect WorldSpaceToScreenSpace(const Rect& worldSpace) const;
	[[nodiscard]] Vector WorldSpaceToScreenSpace(const Vector& worldSpace) const;
	[[nodiscard]] Vector ScreenSpaceToWorldSpace(const Vector& screenSpace) const;
	[[nodiscard]] Vector VectorWorldSpaceToScreenSpace(const Vector& worldSpace) const;

	[[nodiscard]] Vector WorldSpaceToViewportSpace(const Vector& worldSpace) const;
	[[nodiscard]] Vector ViewportSpaceToWorldSpace(const Vector& viewportSpace) const;

	SDL_Texture* CreateTextureFromSurface(SDL_Surface* surface);

	~SceneRenderer();

private:
	struct Vertex
	{
		Vector pos;
		Rgb8 color;

		constexpr Vertex(const Vector& pos, const Rgb8& color)
			: pos(pos)
			, color(color)
		{}
	};

	struct TriangleList
	{
		std::vector<Vertex> vertices;
		std::vector<int> indices;
	};

private:
	[[nodiscard]] Rect WorldSpaceToViewportSpace(const Rect& worldSpace) const;
	[[nodiscard]] Vector VectorWorldSpaceToViewportSpace(const Vector& world_space) const;

	[[nodiscard]] Rect ViewportSpaceToScreenSpace(const Rect& viewportSpace) const;
	[[nodiscard]] Vector ViewportSpaceToScreenSpace(const Vector& viewportSpace) const;
	[[nodiscard]] Vector ScreenSpaceToViewportSpace(const Vector& screenSpace) const;
	[[nodiscard]] Vector VectorViewportSpaceToScreenSpace(const Vector& viewport_space) const;

	bool LoadCharsets();
	VectorInt GetCharCoordinates(char c) const;
	int RenderTriangles(const TriangleList& triangles);

	[[nodiscard]] RenderTargetScope SetTargetLayer(rendering::LayerId layer_id);

	static void RectToTriangles(const DirectedRect& rect, const Rgb8& color, TriangleList& triangles);
	static void VerticesToSDLVertices(const std::vector<Vertex>& vertices, std::vector<SDL_Vertex>& sdl_vertices);
	static SDL_FPoint VectorToSDLPoint(const Vector& v);
	static SDL_Color Rgb8ToSDLColor(const Rgb8& color);

private:
	rendering::TargetLayersContainer m_TargetLayers;

	SDL_Renderer* m_Renderer = nullptr;

	SDL_Surface* m_Charset = nullptr;
	SDL_Surface* m_BigCharset = nullptr;
	SDL_Texture* m_CharsetTex = nullptr;

	Vector m_RTSize;
	Rect m_CurrentViewport;
};

