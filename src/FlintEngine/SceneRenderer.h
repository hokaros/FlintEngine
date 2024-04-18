#pragma once
#include <SDL.h>
#include <SDL_main.h>
#include "Draw.h"
#include "Vector.h"

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

	SDL_Texture* GetOutputTexture() const;

	void RenderTexture(SDL_Texture* texture, const Rect& rect, double angle);
	void RenderRect(const Rect& rect, const Rgb8& color);
	void RenderLine(const Vector& start, const Vector& end, const Rgb8& color);
	void RenderWireRect(const Rect& rect, const Rgb8& color);

	void Clear(const Rgb8& clear_color);

	void SetViewport(const Rect& viewport);
	const Rect& GetRenderedRect() const;
	Rect& GetViewport();

	// narysowanie napisu txt na ekranie, zaczynaj¹c od punktu (x, y)
	void DrawStringScreenSpace(int x, int y, const char* text, int fontSize);
	void RenderTextureScreenSpace(SDL_Texture* texture, const Rect& rect, double angle);

	Rect WorldSpaceToScreenSpace(const Rect& worldSpace) const;
	Vector WorldSpaceToScreenSpace(const Vector& worldSpace) const;

	~SceneRenderer();

private:
	Rect WorldSpaceToViewportSpace(const Rect& worldSpace) const;
	Vector WorldSpaceToViewportSpace(const Vector& worldSpace) const;

	Rect ViewportSpaceToScreenSpace(const Rect& viewportSpace) const;
	Vector ViewportSpaceToScreenSpace(const Vector& viewportSpace) const;

	bool LoadCharsets();
	VectorInt GetCharCoordinates(char c) const;

private:
	SDL_Renderer* m_Renderer = nullptr;
	SDL_Texture* m_OutTexture = nullptr;

	SDL_Surface* m_Charset = nullptr;
	SDL_Surface* m_BigCharset = nullptr;
	SDL_Texture* m_CharsetTex = nullptr;

	Vector m_RTSize;
	Rect m_CurrentViewport;
};

