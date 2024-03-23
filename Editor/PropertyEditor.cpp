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
        }
    }


    ImGui::End();
}

void PropertyEditor::OnObjectSelected(std::weak_ptr<EditorUniversalHandle> object)
{
    m_GameObjectHandle = object;
    
    ApplyObjectToEditors(m_GameObjectHandle);

    m_CurrEditor = GetValidEditor();
}

void PropertyEditor::ApplyObjectToEditors(std::weak_ptr<EditorUniversalHandle> object)
{
    m_GameObjectEditor.SetGameObject(object);
}

IPropertyEditor* PropertyEditor::GetValidEditor()
{
    if (m_GameObjectEditor.HasValidObject())
        return &m_GameObjectEditor;

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
