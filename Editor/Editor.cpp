#include "Editor.h"

Editor::Editor(ImVec4& clear_color, SDL_Renderer& renderer, int screenWidth, int screenHeight)
    : m_ClearColor(clear_color)
    , m_SceneEditor(renderer, screenWidth, screenHeight)
{
    m_GameObjectEditor.Init(m_SelectedGameObjectManager);
    m_HierarchyEditor.Init(m_SelectedGameObjectManager);

    m_SelectedGameObjectManager.OnSceneLoaded(m_SceneEditor.GetScene()); // TODO: do this on every scene load

    m_AssetExplorer.RegisterAssetListener(this);
}

void Editor::Render()
{
    RenderEditorConfigWindow();

    m_GameObjectEditor.Render();
    m_AssetExplorer.Render();
    m_SceneEditor.Render();
    m_HierarchyEditor.Render();
}

void Editor::OnPrefabOpened(std::unique_ptr<EditorPrefabHandle> prefab)
{
    std::shared_ptr<EditorPrefabHandle> prefab_shared = std::shared_ptr<EditorPrefabHandle>(std::move(prefab));
    m_HierarchyEditor.SetGameObject(prefab_shared);
    m_SelectedGameObjectManager.SelectGameObject(prefab_shared);
}

void Editor::RenderEditorConfigWindow()
{
    ImGuiIO& io = ImGui::GetIO();

    ImGui::Begin("Settings");

    ImGui::ColorEdit3("clear color", (float*)&m_ClearColor);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
}
