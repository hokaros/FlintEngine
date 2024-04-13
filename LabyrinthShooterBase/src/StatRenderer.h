#pragma once
#include <Window.h>
#include <Vector.h>
#include <SceneRenderer.h>

// Element wy�wietlaj�cy pewn� statystyk� (zdrowie / liczb� graczy itp.)
class StatRenderer
{
public:
	virtual void Render(SceneRenderer& renderer) = 0;

	void UpdateStat(int newStat);

protected:
	int stat = 0;
};

// Element reprezentuj�cy statystyk�, mno��c elementy graficzne (wy�wietlanie serc itp.)
class BMPStats : public StatRenderer 
{
public:
	BMPStats(SDL_Surface* bitmap, const VectorInt& elementSize, const VectorInt& startPosition);

	void Render(SceneRenderer& renderer) override;

private:
	SDL_Texture* m_Bitmap;
	VectorInt m_ElementSize;
	VectorInt m_StartPos;
};

