#pragma once
#include <SDL.h>
#include <SDL_main.h>
#include "Draw.h"
#include "Vector.h"

class Window;
class SceneEditor;

class RenderingKey
{
private:
	RenderingKey() = default;
	
	friend class Window;
	friend class SceneEditor;
};

class SceneRenderer
{
public:
	static SceneRenderer* Main();

	SceneRenderer(int windowWidth, int windowHeight); // TODO: don't require windowWidth and windowHeight here, just set the render target for the SceneRenderer before any rendering
	bool Init(SDL_Renderer* renderer, RenderingKey);

	SDL_Texture* GetOutputTexture() const;

	void RenderTexture(SDL_Texture* texture, const SDL_Rect& rect, double angle);
	void RenderRect(const Rect& rect, const Rgb8& color);
	void RenderLine(const Vector& start, const Vector& end, const Rgb8& color);
	void RenderWireRect(const Rect& rect, const Rgb8& color);

	Rect GetRenderedRect() const;

	// narysowanie napisu txt na ekranie, zaczynaj¹c od punktu (x, y)
	void DrawStringScreenSpace(int x, int y, const char* text, int fontSize);

	~SceneRenderer();
private:
	bool LoadCharsets();
	VectorInt GetCharCoordinates(char c) const;

private:
	SDL_Renderer* m_Renderer = nullptr;
	SDL_Texture* m_OutTexture = nullptr;

	SDL_Surface* m_Charset = nullptr;
	SDL_Surface* m_BigCharset = nullptr;
	SDL_Texture* m_CharsetTex = nullptr;

	int m_Width;
	int m_Height;

	static SceneRenderer* s_Instance;
};

