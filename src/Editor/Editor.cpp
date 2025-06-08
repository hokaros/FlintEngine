#include "Editor.h"

Editor::Editor(ImVec4& clear_color, SDL_Renderer& renderer, int screenWidth, int screenHeight)
    : m_ClearColor(clear_color)
    , m_SceneEditor(renderer, screenWidth, screenHeight)
    , m_GameRunner(renderer, screenWidth, screenHeight)
{
    m_PropertyEditor.Init(m_SelectedObjectManager);
    m_HierarchyEditor.Init(m_SelectedObjectManager);
    m_SceneEditor.Init(m_SelectedObjectManager);

    m_AssetExplorer.RegisterAssetListener(this);
}

void Editor::Render()
{
    BeginEditorSpace();

    RenderEditorConfigWindow();

    m_PropertyEditor.Render();
    m_AssetExplorer.Render();
    m_SceneEditor.Render();
    m_HierarchyEditor.Render();
    m_GameRunner.Render();

    EndEditorSpace();
}

void Editor::ProcessEvent(const SDL_Event& event)
{
    m_GameRunner.ProcessEvent(event);
}

EditorGameRunner& Editor::GetGameRunner()
{
    return m_GameRunner;
}

void Editor::OnPrefabOpened(std::unique_ptr<EditorPrefabHandle> prefab)
{
    std::shared_ptr<EditorPrefabHandle> prefab_shared = std::shared_ptr<EditorPrefabHandle>(std::move(prefab));
    std::shared_ptr<EditorUniversalHandle> handle = std::make_shared<EditorUniversalHandle>(prefab_shared);

    m_HierarchyEditor.SetEditedObject(handle);
    m_SelectedObjectManager.SelectObject(handle);
    m_SceneEditor.SetRootObject(std::weak_ptr<EditorUniversalHandle>(handle));
    m_GameRunner.SetSelectedScene(handle);
}

void Editor::OnSceneOpened(std::unique_ptr<EditorSceneHandle> scene)
{
    std::shared_ptr<EditorSceneHandle> scene_shared = std::shared_ptr<EditorSceneHandle>(std::move(scene));
    std::shared_ptr<EditorUniversalHandle> handle = std::make_shared<EditorUniversalHandle>(scene_shared);

    m_HierarchyEditor.SetEditedObject(handle);
    m_SelectedObjectManager.SelectObject(handle);
    m_SceneEditor.SetRootObject(std::weak_ptr<EditorUniversalHandle>(handle));
    m_GameRunner.SetSelectedScene(handle);
}

void Editor::BeginEditorSpace()
{
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Editor space", nullptr, window_flags);
    ImGui::PopStyleVar();

    ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("Main dockspace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
}

void Editor::EndEditorSpace()
{
    ImGui::End();
}

void Editor::RenderEditorConfigWindow()
{
    ImGuiIO& io = ImGui::GetIO();

    ImGui::Begin("Settings");

    ImGui::ColorEdit3("clear color", (float*)&m_ClearColor);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
}
