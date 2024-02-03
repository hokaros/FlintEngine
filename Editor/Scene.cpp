#include "Scene.h"

void Scene::Render()
{
	for (GameObject& go : m_GameObjects)
	{
		go.RenderUpdate();
	}

	// Wyœwietlenie ImGuiowego okienka sceny
	// Render to texture
}
