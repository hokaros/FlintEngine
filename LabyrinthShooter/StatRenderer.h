#pragma once
#include "Window.h"
#include "Vector.h"

// Element wyœwietlaj¹cy pewn¹ statystykê (zdrowie / liczbê graczy itp.)
class StatRenderer
{
public:
	virtual void Render() = 0;

	void UpdateStat(int newStat);

protected:
	int stat = 0;
};

// Element reprezentuj¹cy statystykê, mno¿¹c elementy graficzne (wyœwietlanie serc itp.)
class BMPStats : public StatRenderer 
{
public:
	BMPStats(SDL_Surface* bitmap, const VectorInt& elementSize, const VectorInt& startPosition);

	void Render() override;

private:
	SDL_Surface* bitmap;
	VectorInt elementSize;
	VectorInt startPos;
};

