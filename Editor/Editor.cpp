#include "Editor.h"

Editor::Editor(ImVec4& clear_color)
    : m_ClearColor(clear_color)
{
    std::unique_ptr<EditorGameObjectHandle> prefab_handle = OpenPrefab("test.prefab");
    m_GameObjectEditor.SetGameObject(std::move(prefab_handle));
}

void Editor::Render()
{
    RenderEditorConfigWindow();

    m_GameObjectEditor.Render();
}

void Editor::RenderEditorConfigWindow()
{
    ImGuiIO& io = ImGui::GetIO();

    ImGui::Begin("Settings");

    ImGui::ColorEdit3("clear color", (float*)&m_ClearColor);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
}

std::unique_ptr<EditorPrefabHandle> Editor::OpenPrefab(const std::string& prefab_path)
{
    std::unique_ptr<GameObject> prefab = PrefabLoader::LoadPrefab(prefab_path.c_str());
    if (prefab == nullptr)
    { // Make new prefab if file not present
        prefab = std::make_unique<GameObject>(PrefabCreationKey());
    }

    return std::make_unique<EditorPrefabHandle>(std::move(prefab), prefab_path);
}
