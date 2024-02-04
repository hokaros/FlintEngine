#include "Editor.h"

Editor::Editor(ImVec4& clear_color, SDL_Renderer& renderer, int screenWidth, int screenHeight)
    : m_ClearColor(clear_color)
    , m_SceneEditor(renderer, screenWidth, screenHeight)
{
    m_AssetExplorer.RegisterAssetListener(this);
}

void Editor::Render()
{
    RenderEditorConfigWindow();

    m_GameObjectEditor.Render();
    m_AssetExplorer.Render();
    m_SceneEditor.Render();
}

void Editor::OnPrefabOpened(std::unique_ptr<EditorPrefabHandle> prefab)
{
    m_GameObjectEditor.SetGameObject(std::move(prefab));
}

void Editor::RenderEditorConfigWindow()
{
    ImGuiIO& io = ImGui::GetIO();

    ImGui::Begin("Settings");

    ImGui::ColorEdit3("clear color", (float*)&m_ClearColor);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
}
