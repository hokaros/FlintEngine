#include "EditorUniversalHandle.h"


EditorUniversalHandle::EditorUniversalHandle(std::shared_ptr<EditorGameObjectHandle> game_object)
    : m_EditableGameObject(std::move(game_object))
{
    FE_ASSERT(m_EditableGameObject != nullptr, "Creating handle without GameObject");

    m_HierarchyEditable = &m_EditableGameObject->GetGameObject();
    m_Saveable = m_EditableGameObject.get();
}

EditorUniversalHandle::EditorUniversalHandle(std::shared_ptr<EditorIEditableGameObjectHandle> game_object)
    : m_EditableGameObject(std::static_pointer_cast<EditorGameObjectHandle>(game_object))
{
    FE_ASSERT(m_EditableGameObject != nullptr, "Creating handle without GameObject");

    m_HierarchyEditable = &m_EditableGameObject->GetGameObject();
    m_Saveable = m_EditableGameObject.get();
}

EditorUniversalHandle::EditorUniversalHandle(std::shared_ptr<EditorSceneHandle> scene)
    : m_SceneHandle(std::move(scene))
{
    FE_ASSERT(m_SceneHandle != nullptr, "Creating handle without SceneHandle");

    m_HierarchyEditable = m_SceneHandle.get();
    m_Saveable = m_SceneHandle.get();
}

IHierarchyEditable* EditorUniversalHandle::GetHierarchyEditable() const
{
    return m_HierarchyEditable;
}

EditorGameObjectHandle* EditorUniversalHandle::GetGameObjectHandle() const
{
    return m_EditableGameObject.get();
}

EditorSceneHandle* EditorUniversalHandle::GetSceneHandle() const
{
    return m_SceneHandle.get();
}

void EditorUniversalHandle::Save()
{
    m_Saveable->Save();
}

bool EditorUniversalHandle::HasUnsavedChanges() const
{
    return m_Saveable->HasUnsavedChanges();
}

void EditorUniversalHandle::OnUnsavedChange()
{
    return m_Saveable->OnUnsavedChange();
}

bool EditorUniversalHandle::operator==(const EditorUniversalHandle& other) const
{
    return AreGameObjectHandlesSame(other)
        && AreSceneHandlesSame(other);
}

bool EditorUniversalHandle::AreGameObjectHandlesSame(const EditorUniversalHandle& other) const
{
    return ArePointedObjectsEqual(m_EditableGameObject.get(), other.m_EditableGameObject.get());
}

bool EditorUniversalHandle::AreSceneHandlesSame(const EditorUniversalHandle& other) const
{
    return ArePointedObjectsEqual(m_SceneHandle.get(), other.m_SceneHandle.get());
}
