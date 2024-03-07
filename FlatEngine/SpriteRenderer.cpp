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
	Rect dstRect = Rect(m_GameObject->GetPosition(), m_GameObject->GetSize());

	SceneRenderer::Main()->RenderTexture(m_Texture, dstRect, m_GameObject->GetRotation());
}