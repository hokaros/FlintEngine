#include "SpriteRenderer.h"
#include "Draw.h"
#include <Core/GameObject.h>
#include "AssetManager.h"
#include "SceneRenderer.h"

DEFINE_COMPONENT(SpriteRenderer);

DEFINE_FIELD(SpriteRenderer, m_Layer);
DEFINE_FIELD(SpriteRenderer, m_BitmapPath);

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

	const Vector& world_pos = m_GameObject->GetWorldPosition();
	const Vector& world_size = m_GameObject->GetWorldScale();
	const Rect dstRect = Rect(world_pos, world_size);

	renderer.RenderTexture(m_Texture, dstRect, m_GameObject->GetWorldRotation(), m_Layer);
}

SDL_Texture* SpriteRenderer::CreateTextureFromBitmap(const std::string& bitmap_path, SceneRenderer& renderer)
{
	if (bitmap_path.empty())
		return nullptr;

	SDL_Surface* bitmap = AssetManager::GetInstance()->GetSurfaceAsset(bitmap_path);
	return renderer.CreateTextureFromSurface(bitmap);
}
