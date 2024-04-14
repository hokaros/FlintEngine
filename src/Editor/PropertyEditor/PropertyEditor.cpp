#include "PropertyEditor.h"

void PropertyEditor::Init(SelectedObjectManager& selected_game_object_manager)
{
    selected_game_object_manager.SubscribeSelection(*this);
}

void PropertyEditor::Render()
{
    if (ImGui::Begin("Property Editor"))
    {
        ValidateCurrentEditor();

        if (m_CurrEditor == nullptr)
        {
            ImGui::Text("Nothing to see here");
        }
        else
        {
            m_CurrEditor->RenderEmbedded();
            RenderSharedSection();
        }
    }

    ImGui::End();
}

void PropertyEditor::OnObjectSelected(std::weak_ptr<EditorUniversalHandle> object)
{
    m_EditedObjectHandle = object;
    
    ApplyObjectToEditors(m_EditedObjectHandle);

    m_CurrEditor = GetValidEditor();
}

void PropertyEditor::ApplyObjectToEditors(std::weak_ptr<EditorUniversalHandle> object)
{
    m_GameObjectEditor.SetGameObject(object);
    m_SceneEditor.SetScene(object);
}

IPropertyEditor* PropertyEditor::GetValidEditor()
{
    if (m_GameObjectEditor.HasValidObject())
        return &m_GameObjectEditor;

    if (m_SceneEditor.HasValidObject())
        return &m_SceneEditor;

    return nullptr;
}

void PropertyEditor::ValidateCurrentEditor()
{
    if (m_CurrEditor == nullptr)
        return;

    if (m_CurrEditor->HasValidObject() == false)
    {
        m_CurrEditor = nullptr;
    }
}

void PropertyEditor::RenderSharedSection()
{
    if (m_CurrEditor == nullptr)
        return;

    std::shared_ptr<EditorUniversalHandle> edited_object = m_EditedObjectHandle.lock();
    if (ImGui::Button("Save"))
    {
        edited_object->Save();
    }

    if (edited_object->HasUnsavedChanges())
    {
        ImGui::SameLine();
        ImGui::Text("*");
    }
}