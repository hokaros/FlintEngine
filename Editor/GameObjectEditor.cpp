#include "GameObjectEditor.h"

void GameObjectEditor::Init(SelectedGameObjectManager& selected_game_object_manager)
{
    selected_game_object_manager.SubscribeSelection(*this);
}

void GameObjectEditor::Render()
{
    ImGui::Begin("GameObject Editor");

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

void GameObjectEditor::OnGameObjectSelected(EditorGameObjectHandle* game_object)
{
    m_GameObjectHandle = game_object;

    if (m_GameObjectHandle == nullptr || m_GameObjectHandle->GetGameObject() == nullptr)
        return;

    GameObject& go = m_GameObjectHandle->GetGameObject()->GetResult();
    InitValuesFromGameObject(go);
    LoadComponents(*m_GameObjectHandle->GetGameObject());

    LoadAddableComponents();
}

void GameObjectEditor::LoadAddableComponents()
{
    m_AddableComponentNames.clear();
    m_AddableComponents.clear();

    for (const ComponentDefinition* component : ComponentDefinitionManager::GetInstance().GetAllComponents())
    {
        m_AddableComponentNames.push_back(component->GetFileName().c_str());
        m_AddableComponents.push_back(component);
    }
}

void GameObjectEditor::RenderGameObjectEditor(IEditableGameObject& game_object)
{
    ImGui::InputText("Name", m_GameObjectName, s_NameMaxSize);

    ImGui::DragFloat2("Position", m_GameObjectPosition, s_SlidersSpeed);
    ImGui::DragFloat2("Size", m_GameObjectSize, s_SlidersSpeed);

    RenderComponentEditors();
    RenderComponentAddSection();

    ApplyValuesToGameObject(game_object);

    if (ImGui::Button("Save"))
    {
        m_GameObjectHandle->SaveInlineGameObject();
    }
}

void GameObjectEditor::RenderComponentEditors()
{
    if (!m_AreComponentEditorsValid)
    {
        LoadComponents(*m_GameObjectHandle->GetGameObject());
    }

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

void GameObjectEditor::LoadComponents(IEditableGameObject& game_object)
{
    m_ComponentEditors.clear();

    size_t component_idx = 0;
    for (std::unique_ptr<ObjectComponent>& component : game_object.GetResult().GetAllComponents())
    {
        std::unique_ptr<ComponentEditor> comp_editor = std::make_unique<ComponentEditor>(game_object, *component, component_idx);
        comp_editor->RegisterActionObserver(this);
        m_ComponentEditors.push_back(std::move(comp_editor));

        component_idx++;
    }

    m_AreComponentEditorsValid = true;
}

void GameObjectEditor::AddComponent(const ComponentDefinition* component)
{
    IEditableGameObject& game_object = *m_GameObjectHandle->GetGameObject();
    game_object.AddComponent(component->GetConstructor()());

    m_AreComponentEditorsValid = false;
}

void GameObjectEditor::InitValuesFromGameObject(const GameObject& game_object)
{
    m_GameObjectPosition[0] = game_object.GetPosition().x;
    m_GameObjectPosition[1] = game_object.GetPosition().y;

    m_GameObjectSize[0] = game_object.GetSize().x;
    m_GameObjectSize[1] = game_object.GetSize().y;

    strcpy_s(m_GameObjectName, game_object.GetFileName().c_str());
}

void GameObjectEditor::ApplyValuesToGameObject(IEditableGameObject& game_object)
{
    game_object.SetPosition(Vector(m_GameObjectPosition));

    game_object.SetSize(Vector(m_GameObjectSize));

    std::string name_str = std::string(m_GameObjectName);
    game_object.SetName(name_str);
}

void GameObjectEditor::OnComponentDeleted(size_t index_in_game_object)
{
    IEditableGameObject& game_object = *m_GameObjectHandle->GetGameObject();
    game_object.RemoveComponent(index_in_game_object);

    m_AreComponentEditorsValid = false;
}
