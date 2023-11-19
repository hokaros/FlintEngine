#include "Editor.h"

Editor::Editor(ImVec4& clear_color)
    : m_ClearColor(clear_color)
{
    std::unique_ptr<EditorGameObjectHandle> prefab_handle = OpenPrefab("test.prefab");
    m_GameObjectEditor.SetGameObject(std::move(prefab_handle));
}

void Editor::Render()
{
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (m_ShowDemoWindow)
        ImGui::ShowDemoWindow(&m_ShowDemoWindow);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    RenderStartingWindow();

    // 3. Show another simple window.
    if (m_ShowAnotherWindow)
    {
        RenderAnotherWindow();
    }

    m_GameObjectEditor.Render();
}

void Editor::RenderStartingWindow()
{
    ImGuiIO& io = ImGui::GetIO();

    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    ImGui::Checkbox("Demo Window", &m_ShowDemoWindow);      // Edit bools storing our window open/close state
    ImGui::Checkbox("Another Window", &m_ShowAnotherWindow);

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3("clear color", (float*)&m_ClearColor); // Edit 3 floats representing a color

    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
}

void Editor::RenderAnotherWindow()
{
    ImGui::Begin("Another Window", &m_ShowAnotherWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text("Hello from another window!");
    if (ImGui::Button("Close Me"))
        m_ShowAnotherWindow = false;
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
