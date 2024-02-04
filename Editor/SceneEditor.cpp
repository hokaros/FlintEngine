#include "SceneEditor.h"

#include "../FlatEngine/utility.h"

SceneEditor::SceneEditor(SDL_Renderer& renderer, float screenWidth, float screenHeight)
	: m_SceneRenderer(screenWidth, screenHeight)
{
	m_SceneRenderer.Init(&renderer, {});

	AddExampleObjectsToScene();
}

void SceneEditor::Render()
{
	if (ImGui::Begin("Scene Editor"))
	{
		m_Scene.Render(); // TODO: this is highly obscure that the rendered texture is retrieved from the SceneRenderer (maybe we should pass the renderer to GameObject::RenderUpdate())

		if (SDL_Texture* renderedTex = m_SceneRenderer.GetOutputTexture())
		{
			ImVec2 renderSize = ImGui::GetContentRegionAvail();
			ImGui::Image(renderedTex, renderSize);
		}
		else
		{
			FE_ASSERT(false, "No scene rendered");
		}
	}
	ImGui::End(); // Scene Editor
}

void SceneEditor::AddExampleObjectsToScene()
{
	// TODO
}
