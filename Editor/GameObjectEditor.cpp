#include "GameObjectEditor.h"

void GameObjectEditor::Render()
{
    ImGui::Begin("Game Object Editor");

    if (m_GameObjectHandle != nullptr && m_GameObjectHandle->GetGameObject() != nullptr)
    {
        RenderGameObjectEditor(*m_GameObjectHandle->GetGameObject());
    }
    else
    {
        ImGui::Text("No GameObject selected");
    }

    ImGui::End();
}

void GameObjectEditor::SetGameObject(std::unique_ptr<EditorGameObjectHandle> game_object)
{
    m_GameObjectHandle = std::move(game_object);

    if (m_GameObjectHandle == nullptr)
        return;

    GameObject& go = *m_GameObjectHandle->GetGameObject();
    InitValuesFromGameObject(go);
    LoadComponents(go);

    LoadAddableComponents();
}

void GameObjectEditor::LoadAddableComponents()
{
    m_AddableComponentNames.clear();
    m_AddableComponents.clear();

    for (const ComponentDefinition* component : ComponentDefinitionManager::GetInstance().GetAllComponents())
    {
        m_AddableComponentNames.push_back(component->GetName().c_str());
        m_AddableComponents.push_back(component);
    }
}

void GameObjectEditor::RenderGameObjectEditor(GameObject& game_object)
{
    ImGui::DragFloat2("Position", m_GameObjectPosition, s_SlidersSpeed);
    ImGui::DragFloat2("Size", m_GameObjectSize, s_SlidersSpeed);

    RenderComponentEditors();
    RenderComponentAddSection();

    ApplyValuesToGameObject(game_object);

    if (ImGui::Button("Save"))
        m_GameObjectHandle->SaveGameObject();
}

void GameObjectEditor::RenderComponentEditors()
{
    for (std::unique_ptr<ComponentEditor>& comp_editor : m_ComponentEditors)
    {
        ImGui::Separator();
        comp_editor->Render();
    }
    ImGui::Separator();
}

void GameObjectEditor::RenderComponentAddSection()
{
    static int s_ItemCurrent = 0;
    ImGui::Combo("New component", &s_ItemCurrent, m_AddableComponentNames.data(), m_AddableComponentNames.size());
    ImGui::SameLine();
    if (ImGui::Button("+"))
    {
        AddComponent(m_AddableComponents[s_ItemCurrent]);
    }
}

void GameObjectEditor::LoadComponents(GameObject& game_object)
{
    m_ComponentEditors.clear();

    for (ObjectComponent* component : game_object.GetAllComponents())
    {
        std::unique_ptr<ComponentEditor> comp_editor = std::make_unique<ComponentEditor>(*component);
        m_ComponentEditors.push_back(std::move(comp_editor));
    }
}

void GameObjectEditor::AddComponent(const ComponentDefinition* component)
{
    GameObject& game_object = *m_GameObjectHandle->GetGameObject();
    game_object.AddComponent(component->GetConstructor()());

    LoadComponents(game_object);
}

void GameObjectEditor::InitValuesFromGameObject(const GameObject& game_object)
{
    m_GameObjectPosition[0] = game_object.GetPosition().x;
    m_GameObjectPosition[1] = game_object.GetPosition().y;

    m_GameObjectSize[0] = game_object.GetSize().x;
    m_GameObjectSize[1] = game_object.GetSize().y;
}

void GameObjectEditor::ApplyValuesToGameObject(GameObject& game_object)
{
    game_object.SetPosition(Vector(m_GameObjectPosition));

    game_object.SetSize(Vector(m_GameObjectSize));
}



EditorGameObjectHandle::EditorGameObjectHandle(GameObject* game_object)
    : m_GameObject(game_object)
{
}

GameObject* EditorGameObjectHandle::GetGameObject() const
{
    return m_GameObject;
}

void EditorGameObjectHandle::SaveGameObject()
{
}
