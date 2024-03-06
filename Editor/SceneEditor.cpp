#include "SceneEditor.h"

#include "../FlatEngine/utility.h"
#include "../FlatEngine/RectangleRenderer.h"
#include "../FlatEngine/AssetManager.h"

SceneEditor::SceneEditor(SDL_Renderer& renderer, float screenWidth, float screenHeight)
	: m_SceneRenderer(screenWidth, screenHeight)
{
	m_SceneRenderer.Init(&renderer, {});

	AddExampleObjectsToScene();
}

void SceneEditor::SetRootObject(std::weak_ptr<EditorPrefabHandle> root_object)
{
	m_Scene.ResetScene();

	m_RootObject = root_object;

	m_Scene.GetObjectManager().AddToMessageSubscribers(root_object.lock()->GetGameObject());
}

void SceneEditor::Render()
{
	if (m_RootObject.expired())
	{
		ResetRootObject();
	}

	if (ImGui::Begin("Scene Editor"))
	{
		m_Scene.Render(); // TODO: this is highly obscure that the rendered texture is retrieved from the SceneRenderer (maybe we should pass the renderer to GameObject::RenderUpdate())

		if (SDL_Texture* renderedTex = m_SceneRenderer.GetOutputTexture())
		{
			ImVec2 renderSize = ImGui::GetContentRegionAvail();
			ImGui::Image((void*)renderedTex, renderSize);
		}
		else
		{
			FE_ASSERT(false, "No scene rendered");
		}
	}
	ImGui::End(); // Scene Editor
}

Scene& SceneEditor::GetScene()
{
	return m_Scene;
}

void SceneEditor::AddExampleObjectsToScene()
{
	GameObject* game_object = GameObject::Instantiate(m_Scene);

	game_object->SetSize(Vector(10, 10));
	game_object->SetPosition(Vector(50, 50));
	game_object->AddComponent(std::make_unique<RectangleRenderer>(Rgb8(0xFF, 0x00, 0x00)));
}

void SceneEditor::ResetRootObject()
{
	m_Scene.ResetScene();

	m_RootObject = std::weak_ptr<EditorPrefabHandle>();
}
