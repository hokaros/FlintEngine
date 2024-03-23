#include "ScenePropertyEditor.h"

#include "../FlatEngine/imgui/imgui.h"

void ScenePropertyEditor::RenderEmbedded()
{
    EditorSceneHandle* scene_handle = GetSceneHandle();
    if (scene_handle != nullptr && scene_handle->GetScene() != nullptr)
    {
        ImGui::Text("Scene selected!");
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

    EditableScene& scene = *scene_handle->GetScene();
    // TODO
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

    std::shared_ptr<EditorSceneHandle> go_handle = shared_obj->GetSceneHandle();
    return go_handle.get();
}
