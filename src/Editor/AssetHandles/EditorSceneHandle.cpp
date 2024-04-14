#include "EditorSceneHandle.h"

#include <Serialization/SceneSaver.h>



EditorSceneHandle::EditorSceneHandle(std::unique_ptr<EditableScene> scene, const std::string& scene_path)
    : m_Scene(std::move(scene))
    , m_ScenePath(scene_path)
{
    FE_ASSERT(m_Scene != nullptr, "No scene passed to SceneHandle");
}

EditableScene& EditorSceneHandle::GetScene() const
{
    return *m_Scene;
}

void EditorSceneHandle::Save()
{
    if (m_Scene != nullptr)
    {
        SceneSaver::SaveScene(*m_Scene, m_ScenePath.c_str());
    }

    m_HasUnsavedChanges = false;
}

bool EditorSceneHandle::HasUnsavedChanges() const
{
    return m_HasUnsavedChanges;
}

void EditorSceneHandle::OnUnsavedChange()
{
    m_HasUnsavedChanges = true;
}

const std::vector<std::unique_ptr<IEditableGameObject>>& EditorSceneHandle::GetSubRootObjects() const
{
    return m_Scene->GetRootObjects();
}

void EditorSceneHandle::AddChild(std::unique_ptr<IEditableGameObject> child)
{
    m_Scene->AddRootObject(std::move(child));
    OnUnsavedChange();
}

void EditorSceneHandle::DeleteChild(IEditableGameObject& child)
{
    m_Scene->RemoveRootObject(child);
    OnUnsavedChange();
}

const char* EditorSceneHandle::GetName() const
{
    return m_Scene->GetName();
}

bool EditorSceneHandle::operator==(const EditorSceneHandle& other) const
{
    return m_Scene.get() == other.m_Scene.get();
}
