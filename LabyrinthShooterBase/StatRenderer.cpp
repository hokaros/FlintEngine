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
	Rect dest = Rect(m_StartPos, m_ElementSize);

	for (int i = 0; i < stat; i++) 
	{
		SceneRenderer::Main()->RenderTexture(m_Bitmap, dest, /* angle = */0.0f);

		dest.pos.x += m_ElementSize.x;
	}
}