#pragma once
#include <SDL.h>
#include <SDL_main.h>

#include <map>
#include <functional>
#include <Math/Vector.h>
#include "RenderingHelpers.h"
#include "LayerId.h"
#include <SimpleTypes.h>

namespace rendering
{
	class TargetLayersContainer
	{
	public:
		TargetLayersContainer(const VectorInt& target_size);
		~TargetLayersContainer();

		void Init(SDL_Renderer* renderer);

		SDL_Texture* GetOrCreateLayer(LayerId layer);
		void ClearLayers();
		SDL_Texture* MergeLayers(); // Merges layers to layer 0 and returns it

	private:
		SDL_Texture* CreateLayer();
		void ClearLayer(SDL_Texture* layer);

	private:
		std::map<uint, SDL_Texture*> m_Layers;
		VectorInt m_TargetSize;

		SDL_Renderer* m_Renderer;
	};
}