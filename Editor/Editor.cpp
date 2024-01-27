#include "Editor.h"

Editor::Editor(ImVec4& clear_color)
    : m_ClearColor(clear_color)
{
    m_AssetExplorer.RegisterAssetListener(this);
}

void Editor::Render()
{
    RenderEditorConfigWindow();

    m_GameObjectEditor.Render();
    m_AssetExplorer.Render();
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
