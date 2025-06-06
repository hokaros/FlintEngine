#pragma once
#include <Math/Vector.h>
#include <Rendering/SceneRenderer.h>
#include <ComponentDefinition.h>

// Element wyświetlający pewną statystykę (zdrowie / liczbę graczy itp.)
class StatRenderer
{
public:
	virtual void Render(SceneRenderer& renderer) = 0;

	void UpdateStat(int newStat);

protected:
	int stat = 0;
};

// Element reprezentujący statystykę, mnożąc elementy graficzne (wyświetlanie serc itp.)
class BMPStats 
	: public ObjectComponent
	, public StatRenderer // TODO: why do we need this order of base classes?
{
	DECLARE_COMPONENT();
public:
	BMPStats() = default;
	~BMPStats();

	virtual void Render(SceneRenderer& renderer) override;
	virtual void RenderUpdate(SceneRenderer& renderer) override;

private:
	static SDL_Texture* CreateTextureFromBitmap(const std::string& bitmap_path, SceneRenderer& renderer);

private:
	SDL_Texture* m_Bitmap = nullptr;

	uint m_Layer = 0;
	DECLARE_FIELD(m_Layer);

	std::string m_BitmapPath = "";
	DECLARE_FIELD(m_BitmapPath);

	VectorInt m_ElementSize;
	DECLARE_FIELD(m_ElementSize);

	VectorInt m_StartPos;
	DECLARE_FIELD(m_StartPos);
};

