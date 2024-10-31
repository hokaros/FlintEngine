#include "ScenePropertyEditor.h"

#include <imgui/imgui.h>

void ScenePropertyEditor::RenderEmbedded()
{
    EditorSceneHandle* scene_handle = GetSceneHandle();
    if (scene_handle != nullptr)
    {
        RenderSceneEditor(scene_handle->GetScene());
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

void ScenePropertyEditor::RenderSceneEditor(Scene& scene)
{
    ImGui::Text("Scene properties");
    RenderActions(scene);

    ApplyValuesToScene(scene);
}

void ScenePropertyEditor::InitValuesFromScene(Scene& scene)
{
}

void ScenePropertyEditor::ApplyValuesToScene(Scene& scene)
{

}

void ScenePropertyEditor::RenderActions(Scene& scene)
{
    if (ImGui::Button("Generate Navmesh"))
    {
        scene.RegenerateNavmesh();
    }
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
