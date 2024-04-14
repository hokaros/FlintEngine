#include "SpriteRenderer.h"
#include "Draw.h"
#include <Core/GameObject.h>
#include "AssetManager.h"
#include "SceneRenderer.h"

DEFINE_COMPONENT(SpriteRenderer);

DEFINE_FIELD(SpriteRenderer, m_BitmapPath);

SpriteRenderer::SpriteRenderer(const std::string& bitmap_path)
	: m_BitmapPath(bitmap_path)
{

}

SpriteRenderer::~SpriteRenderer() 
{
	if (m_Texture != nullptr)
	{
		SDL_DestroyTexture(m_Texture);
	}
}

void SpriteRenderer::Render(SceneRenderer& renderer)
{
	if (m_Texture == nullptr)
	{
		m_Texture = CreateTextureFromBitmap(m_BitmapPath, renderer);
	}

	Rect dstRect = Rect(m_GameObject->GetPosition(), m_GameObject->GetSize());

	renderer.RenderTexture(m_Texture, dstRect, m_GameObject->GetRotation());
}

SDL_Texture* SpriteRenderer::CreateTextureFromBitmap(const std::string& bitmap_path, SceneRenderer& renderer)
{
	if (bitmap_path.empty())
		return nullptr;

	SDL_Surface* bitmap = AssetManager::GetInstance()->GetSurfaceAsset(bitmap_path);
	return SDL_CreateTextureFromSurface(renderer.GetRenderer(), bitmap);
}
