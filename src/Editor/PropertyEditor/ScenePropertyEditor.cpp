#include "ScenePropertyEditor.h"

#include <imgui/imgui.h>

void ScenePropertyEditor::RenderEmbedded()
{
    EditorSceneHandle* scene_handle = GetSceneHandle();
    if (scene_handle != nullptr)
    {
        RenderSceneEditor(*scene_handle);
    }
    else
    {
        ImGui::Text("No scene selected");
    }
}

bool ScenePropertyEditor::HasValidObject() const
{
    return GetSceneHandle() != nullptr;
}

void ScenePropertyEditor::SetScene(std::weak_ptr<EditorUniversalHandle> handle)
{
    m_SceneHandle = handle;

    EditorSceneHandle* scene_handle = GetSceneHandle();
    if (scene_handle == nullptr)
        return;

    Scene& scene = scene_handle->GetScene();
    InitValuesFromScene(scene);
}

void ScenePropertyEditor::RenderSceneEditor(EditorSceneHandle& scene)
{
    ImGui::Text("Scene properties");

    ImGui::SeparatorText("Navmesh");
    RenderNavmeshSection(scene.GetScene());
    ImGui::Separator();

    ApplyValuesToScene(scene);
}

void ScenePropertyEditor::InitValuesFromScene(const Scene& scene)
{
    m_NavmeshGenerationParams = scene.GetNavmeshGenerationParams();
}

void ScenePropertyEditor::ApplyValuesToScene(EditorSceneHandle& scene)
{
    if (m_NavmeshGenerationParams != scene.GetScene().GetNavmeshGenerationParams())
    {
        scene.SetNavmeshGenerationParams(m_NavmeshGenerationParams);
    }
}

void ScenePropertyEditor::RenderNavmeshSection(Scene& scene)
{
    RenderNavmeshGenerationParamsConfig();

    if (ImGui::Button("Generate Navmesh"))
    {
        scene.RegenerateNavmesh();
    }
}

void ScenePropertyEditor::RenderNavmeshGenerationParamsConfig()
{
    ImGui::InputFloat("Merge distance", &m_NavmeshGenerationParams.merge_distance);
}

EditorSceneHandle* ScenePropertyEditor::GetSceneHandle()
{
    return const_cast<EditorSceneHandle*>(const_cast<const ScenePropertyEditor*>(this)->GetSceneHandle());
}

const EditorSceneHandle* ScenePropertyEditor::GetSceneHandle() const
{
    std::shared_ptr<EditorUniversalHandle> shared_obj = m_SceneHandle.lock();
    if (shared_obj == nullptr)
        return nullptr;

    return shared_obj->GetSceneHandle();
}
