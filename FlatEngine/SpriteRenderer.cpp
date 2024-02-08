#include "SpriteRenderer.h"
#include "Draw.h"
#include "GameObject.h"
#include "AssetManager.h"

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

void SpriteRenderer::Awake()
{
	if (m_BitmapPath.empty())
	{
		FE_WARN("No bitmap path set");
		return;
	}

	SDL_Surface* bitmap = AssetManager::GetInstance()->GetSurfaceAsset(m_BitmapPath);
	m_Texture = SDL_CreateTextureFromSurface(Window::Main()->GetRenderer(), bitmap);
}

void SpriteRenderer::Render()
{
	SDL_Rect dstRect;
	dstRect.x = m_GameObject->GetPosition().x;
	dstRect.y = m_GameObject->GetPosition().y;
	dstRect.w = m_GameObject->GetSize().x;
	dstRect.h = m_GameObject->GetSize().y;

	SceneRenderer::Main()->RenderTexture(m_Texture, dstRect, m_GameObject->GetRotation());
}

std::unique_ptr<ObjectComponent> SpriteRenderer::Copy() 
{
	return std::make_unique<SpriteRenderer>(m_BitmapPath);
}