#include "SpriteRenderer.h"
#include "Draw.h"
#include <Core/GameObject.h>
#include "AssetManager.h"
#include "SceneRenderer.h"

DEFINE_COMPONENT(SpriteRenderer);

DEFINE_FIELD(SpriteRenderer, m_Layer);
DEFINE_FIELD(SpriteRenderer, m_BitmapPath);
DEFINE_FIELD(SpriteRenderer, m_Size);

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

	renderer.RenderTexture(m_Texture, GetRenderRect(), m_GameObject->GetWorldRotation(), m_Layer);
}

Vector SpriteRenderer::GetRenderSize() const
{
	return m_Size.GetScaled(m_GameObject->GetWorldScale());
}

Rect SpriteRenderer::GetRenderRect() const
{
	const Vector size = GetRenderSize();
	const Vector pos = m_GameObject->GetWorldPosition() - size / 2.f;
	return Rect(pos, size);
}

SDL_Texture* SpriteRenderer::CreateTextureFromBitmap(const std::string& bitmap_path, SceneRenderer& renderer)
{
	if (bitmap_path.empty())
		return nullptr;

	SDL_Surface* bitmap = AssetManager::GetInstance()->GetSurfaceAsset(bitmap_path);
	return renderer.CreateTextureFromSurface(bitmap);
}
