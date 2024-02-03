#include "Scene.h"

void Scene::Render()
{
	for (GameObject& go : m_GameObjects)
	{
		go.RenderUpdate();
	}

	// Wyświetlenie ImGuiowego okienka sceny
	// Render to texture
}
