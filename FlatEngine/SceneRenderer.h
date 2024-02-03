#pragma once
#include <SDL.h>
#include <SDL_main.h>
#include "Draw.h"
#include "Vector.h"

class Window;

class RenderingKey
{
private:
	RenderingKey() = default;
	
	friend class Window;
};

class SceneRenderer
{
public:
	static SceneRenderer* Main();

	bool Init(SDL_Renderer* renderer, RenderingKey);

	void RenderTexture(SDL_Texture* texture, const SDL_Rect& rect, double angle);
	void RenderRect(const Rect& rect, const Rgb8& color);
	void RenderLine(const Vector& start, const Vector& end, const Rgb8& color);
	void RenderWireRect(const Rect& rect, const Rgb8& color);

	// narysowanie napisu txt na ekranie, zaczynaj¹c od punktu (x, y)
	void DrawStringScreenSpace(int x, int y, const char* text, int fontSize);
private:
	bool LoadCharsets();
	VectorInt GetCharCoordinates(char c) const;

private:
	SDL_Renderer* m_Renderer;

	SDL_Surface* m_Charset = nullptr; // TODO: remove
	SDL_Surface* m_BigCharset = nullptr;
	SDL_Texture* m_CharsetTex = nullptr;

	static SceneRenderer* s_Instance;
};

