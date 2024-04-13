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

    EditableScene& scene = scene_handle->GetScene();
    InitValuesFromScene(scene);
}

void ScenePropertyEditor::RenderSceneEditor(EditableScene& scene)
{
    ImGui::Text("Scene properties");

    if (ImGui::Button("Save"))
    {
        GetSceneHandle()->Save();
    }
}

void ScenePropertyEditor::InitValuesFromScene(EditableScene& scene)
{
}

void ScenePropertyEditor::ApplyValuesToScene(EditableScene& scene)
{

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
