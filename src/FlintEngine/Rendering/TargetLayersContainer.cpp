#include "TargetLayersContainer.h"

TargetLayersContainer::TargetLayersContainer(const VectorInt& target_size)
	: m_TargetSize(target_size)
{
}

TargetLayersContainer::~TargetLayersContainer()
{
	for (auto& entry : m_Layers)
	{
		SDL_Texture* texture = entry.second;
		SDL_DestroyTexture(texture);
	}
}

void TargetLayersContainer::Init(SDL_Renderer* renderer)
{
	m_Renderer = renderer;
}

SDL_Texture* TargetLayersContainer::CreateLayer()
{
	SDL_Texture* new_layer = SDL_CreateTexture(m_Renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_TARGET,
		m_TargetSize.x, m_TargetSize.y);

	ClearLayer(new_layer);
	return new_layer;
}

void TargetLayersContainer::ClearLayer(SDL_Texture* layer)
{
	RenderTargetScope rt_scope(m_Renderer, layer);

	SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 0); // Fully transparent
	SDL_RenderClear(m_Renderer);
}

SDL_Texture* TargetLayersContainer::GetOrCreateLayer(uint layer)
{
	auto it = m_Layers.find(layer);
	if (it == m_Layers.end())
	{
		// Insert
		auto p = m_Layers.emplace(layer, CreateLayer());
		return p.first->second;
	}

	return it->second;
}

void TargetLayersContainer::ForEachLayerAscending(std::function<void(SDL_Texture*)> action) const
{
	for (auto it = m_Layers.begin(); it != m_Layers.end(); it++)
	{
		SDL_Texture* layer_target = it->second;
		action(layer_target);
	}
}

void TargetLayersContainer::ClearLayers()
{
	for (auto& layer : m_Layers)
	{
		SDL_Texture* layer_tex = layer.second;

		ClearLayer(layer_tex);
	}
}