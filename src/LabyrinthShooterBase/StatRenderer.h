#pragma once
#include <Vector.h>
#include <SceneRenderer.h>

// Element wyœwietlaj¹cy pewn¹ statystykê (zdrowie / liczbê graczy itp.)
class StatRenderer
{
public:
	virtual void Render(SceneRenderer& renderer) = 0;

	void UpdateStat(int newStat);

protected:
	int stat = 0;
};

// Element reprezentuj¹cy statystykê, mno¿¹c elementy graficzne (wyœwietlanie serc itp.)
class BMPStats : public StatRenderer 
{
public:
	BMPStats(const std::string& bitmap_path, const VectorInt& elementSize, const VectorInt& startPosition);
	~BMPStats();

	void Render(SceneRenderer& renderer) override;

private:
	static SDL_Texture* CreateTextureFromBitmap(const std::string& bitmap_path, SceneRenderer& renderer);

private:
	SDL_Texture* m_Bitmap;
	std::string m_BitmapPath;

	VectorInt m_ElementSize;
	VectorInt m_StartPos;
};

