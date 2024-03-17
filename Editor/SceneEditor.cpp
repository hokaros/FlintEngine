#include "SceneEditor.h"

#include "../FlatEngine/utility.h"
#include "../FlatEngine/RectangleRenderer.h"
#include "../FlatEngine/AssetManager.h"

SceneEditor::SceneEditor(SDL_Renderer& renderer, float screenWidth, float screenHeight)
	: m_SceneRenderer(screenWidth, screenHeight)
{
	m_SceneRenderer.Init(&renderer, {});
	m_SceneRenderer.SetViewport(Rect(Vector(0, 0), Vector(10, 10)));

	AddExampleObjectsToScene();
}

void SceneEditor::SetRootObject(std::weak_ptr<EditorUniversalHandle> root_object)
{
	m_Scene.ResetScene();

	m_RootObject = root_object;

	//m_Scene.GetObjectManager().AddToMessageSubscribers(&root_object.lock()->GetGameObject()->GetResult());
}

void SceneEditor::Render()
{
	if (m_RootObject.expired())
	{
		ResetRootObject();
	}

	if (ImGui::Begin("Scene Editor"))
	{
		if (ImGui::IsWindowFocused())
		{
			viewportController.Update(m_SceneRenderer.GetViewport());
		}
		// WARNING: do not render anything between viewportController's update and displaying the scene

		m_Scene.Render(); // TODO: this is highly obscure that the rendered texture is retrieved from the SceneRenderer (maybe we should pass the renderer to GameObject::RenderUpdate())

		if (std::shared_ptr<EditorUniversalHandle> handle = m_RootObject.lock(); 
			handle != nullptr 
			&& handle->GetGameObjectHandle() != nullptr
			&& handle->GetGameObjectHandle()->GetGameObject() != nullptr)
		{
			IEditableGameObject::RenderUpdate(*handle->GetGameObjectHandle()->GetGameObject());
		}

		if (SDL_Texture* renderedTex = m_SceneRenderer.GetOutputTexture())
		{
			ImVec2 renderSize = ImGui::GetContentRegionAvail();
			ImGui::Image((void*)renderedTex, renderSize);
		}
		else
		{
			FE_ASSERT(false, "No scene rendered");
		}

		ImGui::SetCursorPos(ImGui::GetWindowContentRegionMin());
		RenderOverlay();
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

	m_RootObject = std::weak_ptr<EditorUniversalHandle>();
}

void SceneEditor::RenderOverlay()
{
	Vector viewport_pos = m_SceneRenderer.GetViewport().pos;

	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 0xFF));
	ImGui::Text("(%0.2f, %0.2f)", viewport_pos.x, viewport_pos.y);
	ImGui::PopStyleColor();
}
