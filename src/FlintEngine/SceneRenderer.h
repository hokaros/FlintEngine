#pragma once
#include <Rendering/TargetLayersContainer.h>

#include "Draw.h"

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

	void RenderTexture(SDL_Texture* texture, const Rect& rect, double angle, uint layer);
	void RenderRect(const Rect& rect, const Rgb8& color, uint layer);
	void RenderLine(const Vector& start, const Vector& end, const Rgb8& color, uint layer);
	void RenderWireRect(const Rect& rect, const Rgb8& color, uint layer);

	void RenderString(const char* text, const Vector& start, int fontSize, uint layer); // Renders string in world space. start is the upper left corner

	void Clear(const Rgb8& clear_color);

	void SetViewport(const Rect& viewport);
	const Rect& GetRenderedRect() const;
	Rect& GetViewport();

	// narysowanie napisu txt na ekranie, zaczynaj¹c od punktu (x, y)
	void DrawStringScreenSpace(int x, int y, const char* text, int fontSize, uint layer);
	void RenderTextureScreenSpace(SDL_Texture* texture, const Rect& rect, double angle, uint layer);

	Rect WorldSpaceToScreenSpace(const Rect& worldSpace) const;
	Vector WorldSpaceToScreenSpace(const Vector& worldSpace) const;
	Vector VectorWorldSpaceToScreenSpace(const Vector& worldSpace) const;

	SDL_Texture* CreateTextureFromSurface(SDL_Surface* surface);

	~SceneRenderer();

private:
	Rect WorldSpaceToViewportSpace(const Rect& worldSpace) const;
	Vector WorldSpaceToViewportSpace(const Vector& worldSpace) const;
	Vector VectorWorldSpaceToViewportSpace(const Vector& world_space) const;

	Rect ViewportSpaceToScreenSpace(const Rect& viewportSpace) const;
	Vector ViewportSpaceToScreenSpace(const Vector& viewportSpace) const;
	Vector VectorViewportSpaceToScreenSpace(const Vector& viewport_space) const;

	bool LoadCharsets();
	VectorInt GetCharCoordinates(char c) const;

	[[nodiscard]] RenderTargetScope SetTargetLayer(uint layer_index);

private:
	TargetLayersContainer m_TargetLayers;

	SDL_Renderer* m_Renderer = nullptr;

	SDL_Surface* m_Charset = nullptr;
	SDL_Surface* m_BigCharset = nullptr;
	SDL_Texture* m_CharsetTex = nullptr;

	Vector m_RTSize;
	Rect m_CurrentViewport;
};

