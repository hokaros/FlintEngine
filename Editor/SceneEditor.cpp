#include "SceneEditor.h"

#include "../FlatEngine/utility.h"
#include "../FlatEngine/RectangleRenderer.h"
#include "../FlatEngine/AssetManager.h"

ImU32 Rgb8ToImU32(const Rgb8& rgb)
{
	return IM_COL32(rgb.r, rgb.g, rgb.b, 0xFF);
}

SceneEditor::SceneEditor(SDL_Renderer& renderer, float screenWidth, float screenHeight)
	: m_SceneRenderer(screenWidth, screenHeight)
{
	m_SceneRenderer.Init(&renderer, {});
	m_SceneRenderer.SetViewport(Rect(Vector(0, 0), Vector(10, 10)));

	m_CurrentScene = &m_PrefabScene;
	m_PrefabScene.SetBackgroundColor(Rgb8(200, 200, 200));
}

void SceneEditor::SetRootObject(std::weak_ptr<EditorUniversalHandle> root_object)
{
	m_RootObject = root_object;

	std::shared_ptr<EditorUniversalHandle> handle = m_RootObject.lock();
	EditorSceneHandle* scene = handle->GetSceneHandle();
	if (scene != nullptr)
	{
		m_CurrentScene = &scene->GetScene();
	}
	else
	{
		m_CurrentScene = &m_PrefabScene;
	}
}

void SceneEditor::Render()
{
	FE_ASSERT(m_CurrentScene != nullptr, "Scene should be present. Consider using the default one");

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
		// WARNING: do not render any widget between viewportController's update and displaying the scene - viewportController's calculations may be wrong

		m_CurrentScene->Render(); // TODO: this is highly obscure that the rendered texture is retrieved from the SceneRenderer (maybe we should pass the renderer to GameObject::RenderUpdate())

		if (std::shared_ptr<EditorUniversalHandle> handle = m_RootObject.lock(); 
			handle != nullptr 
			&& handle->GetGameObjectHandle() != nullptr)
		{
			IEditableGameObject::RenderUpdate(handle->GetGameObjectHandle()->GetGameObject());
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

void SceneEditor::ResetRootObject()
{
	m_RootObject = std::weak_ptr<EditorUniversalHandle>();
}

void SceneEditor::RenderOverlay()
{
	Vector viewport_pos = m_SceneRenderer.GetViewport().pos;

	Rgb8 text_col = GetNegativeColor(m_CurrentScene->GetBackgroundColor());
	ImGui::PushStyleColor(ImGuiCol_Text, Rgb8ToImU32(text_col));
	ImGui::Text("(%0.2f, %0.2f)", viewport_pos.x, viewport_pos.y);
	ImGui::PopStyleColor();
}

Rgb8 SceneEditor::GetNegativeColor(const Rgb8& color)
{
	return Rgb8(
		0xFF - color.r,
		0xFF - color.g,
		0xFF - color.b
	);
}
