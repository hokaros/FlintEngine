#include "StatRenderer.h"

void StatRenderer::UpdateStat(int newStat) 
{
	stat = newStat;
}


BMPStats::BMPStats(SDL_Surface* bitmap, const VectorInt& elementSize, const VectorInt& startPosition)
	: m_Bitmap(SDL_CreateTextureFromSurface(Window::Main()->GetRenderer(), bitmap))
	, m_ElementSize(elementSize)
	, m_StartPos(startPosition)
{

}

void BMPStats::Render() 
{
	// Wyœwietl tyle elementów, ile wynosi stat
	SDL_Rect dest;
	dest.x = m_StartPos.x;
	dest.y = m_StartPos.y;
	dest.w = m_ElementSize.x;
	dest.h = m_ElementSize.y;

	for (int i = 0; i < stat; i++) 
	{
		Window::Main()->RenderTexture(m_Bitmap, dest, /* angle = */0.0f);

		dest.x += m_ElementSize.x;
	}
}