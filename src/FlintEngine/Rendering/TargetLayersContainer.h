#pragma once
#include <SDL.h>
#include <SDL_main.h>

#include <map>
#include <functional>
#include <Vector.h>
#include "RenderingHelpers.h"

typedef unsigned int uint;

class TargetLayersContainer
{
public:
	TargetLayersContainer(const VectorInt& target_size);
	~TargetLayersContainer();

	void Init(SDL_Renderer* renderer);

	SDL_Texture* GetOrCreateLayer(uint layer);
	void ForEachLayerAscending(std::function<void(SDL_Texture*)> action) const;
	void ClearLayers();

private:
	SDL_Texture* CreateLayer();
	void ClearLayer(SDL_Texture* layer);

private:
	std::map<uint, SDL_Texture*> m_Layers;
	VectorInt m_TargetSize;

	SDL_Renderer* m_Renderer;
};