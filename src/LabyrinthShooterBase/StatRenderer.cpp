#include "StatRenderer.h"
#include <AssetManager.h>

void StatRenderer::UpdateStat(int newStat) 
{
	stat = newStat;
}


DEFINE_COMPONENT(BMPStats);
DEFINE_FIELD(BMPStats, m_BitmapPath);
DEFINE_FIELD(BMPStats, m_ElementSize);
DEFINE_FIELD(BMPStats, m_StartPos);

BMPStats::~BMPStats()
{
	if (m_Bitmap != nullptr)
	{
		SDL_DestroyTexture(m_Bitmap);
	}
}

void BMPStats::Render(SceneRenderer& renderer)
{
	if (m_Bitmap == nullptr)
	{
		m_Bitmap = CreateTextureFromBitmap(m_BitmapPath, renderer);
	}

	// Wyœwietl tyle elementów, ile wynosi stat
	Rect dest = Rect(m_StartPos, m_ElementSize);

	for (int i = 0; i < stat; i++) 
	{
		renderer.RenderTexture(m_Bitmap, dest, /* angle = */0.0f);

		dest.pos.x += m_ElementSize.x;
	}
}

void BMPStats::RenderUpdate(SceneRenderer& renderer)
{
	Render(renderer);
}

SDL_Texture* BMPStats::CreateTextureFromBitmap(const std::string& bitmap_path, SceneRenderer& renderer)
{
	if (bitmap_path.empty())
		return nullptr;

	SDL_Surface* bitmap = AssetManager::GetInstance()->GetSurfaceAsset(bitmap_path);
	return renderer.CreateTextureFromSurface(bitmap);
}