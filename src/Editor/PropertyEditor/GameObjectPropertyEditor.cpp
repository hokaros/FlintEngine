#include "GameObjectPropertyEditor.h"

void GameObjectPropertyEditor::RenderEmbedded()
{
    EditorGameObjectHandle* game_object_handle = GetGameObjectHandle();
    if (game_object_handle != nullptr)
    {
        RenderGameObjectEditor(game_object_handle->GetGameObject());
    }
    else
    {
        ImGui::Text("No GameObject selected");
    }
}

void GameObjectPropertyEditor::SetGameObject(std::weak_ptr<EditorUniversalHandle> object)
{
    m_GameObjectHandle = object;

    EditorGameObjectHandle* go_handle = GetGameObjectHandle();
    if (go_handle == nullptr)
        return;

    GameObject& go = go_handle->GetGameObject().GetResult();
    InitValuesFromGameObject(go);
    LoadComponents(go_handle->GetGameObject());

    LoadAddableComponents();
}

bool GameObjectPropertyEditor::HasValidObject() const
{
    return GetGameObjectHandle() != nullptr;
}

void GameObjectPropertyEditor::LoadAddableComponents()
{
    m_AddableComponentNames.clear();
    m_AddableComponents.clear();

    for (const ComponentDefinition* component : ComponentDefinitionManager::GetInstance().GetAllComponents())
    {
        m_AddableComponentNames.push_back(component->GetFileName().c_str());
        m_AddableComponents.push_back(component);
    }
}

void GameObjectPropertyEditor::RenderGameObjectEditor(IEditableGameObject& game_object)
{
    ImGui::InputText("Name", m_GameObjectName, s_NameMaxSize);

    ImGui::DragFloat2("Position", m_GameObjectPosition, s_SlidersSpeed);
    ImGui::DragFloat2("Size", m_GameObjectSize, s_SlidersSpeed);

    RenderComponentEditors();
    RenderComponentAddSection();

    ApplyValuesToGameObject(game_object);
}

void GameObjectPropertyEditor::RenderComponentEditors()
{
    if (!m_AreComponentEditorsValid)
    {
        LoadComponents(GetGameObjectHandle()->GetGameObject()); // TODO: let's pass GameObjectHandle as a parameter
    }

    for (std::unique_ptr<ComponentEditor>& comp_editor : m_ComponentEditors)
    {
        ImGui::Separator();

        comp_editor->Render();
    }
    ImGui::Separator();
}

void GameObjectPropertyEditor::RenderComponentAddSection()
{
    static int s_ItemCurrent = 0;
    ImGui::Combo("New component", &s_ItemCurrent, m_AddableComponentNames.data(), m_AddableComponentNames.size());
    ImGui::SameLine();
    if (ImGui::Button("+"))
    {
        AddComponent(m_AddableComponents[s_ItemCurrent]);
    }
}

void GameObjectPropertyEditor::LoadComponents(IEditableGameObject& game_object)
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

void GameObjectPropertyEditor::AddComponent(const ComponentDefinition* component)
{
    IEditableGameObject& game_object = GetGameObjectHandle()->GetGameObject();
    game_object.AddComponent(component->GetConstructor()());

    m_AreComponentEditorsValid = false;
}

void GameObjectPropertyEditor::InitValuesFromGameObject(const GameObject& game_object)
{
    m_GameObjectPosition[0] = game_object.GetPosition().x;
    m_GameObjectPosition[1] = game_object.GetPosition().y;

    m_GameObjectSize[0] = game_object.GetSize().x;
    m_GameObjectSize[1] = game_object.GetSize().y;

    strcpy_s(m_GameObjectName, game_object.GetName().c_str());
}

void GameObjectPropertyEditor::ApplyValuesToGameObject(IEditableGameObject& game_object)
{
    bool any_change = false;

    Vector target_position = Vector(m_GameObjectPosition);
    if (game_object.GetResult().GetPosition() != target_position)
    {
        game_object.SetPosition(target_position);
        any_change = true;
    }

    Vector target_size = Vector(m_GameObjectSize);
    if (game_object.GetResult().GetSize() != target_size)
    {
        game_object.SetSize(target_size);
        any_change = true;
    }

    std::string target_name = std::string(m_GameObjectName);
    if (game_object.GetResult().GetName() != target_name)
    {
        game_object.SetName(target_name);
        any_change = true;
    }

    if (any_change)
    {
        GetGameObjectHandle()->OnUnsavedChange();
    }
}

EditorGameObjectHandle* GameObjectPropertyEditor::GetGameObjectHandle()
{
    return const_cast<EditorGameObjectHandle*>(const_cast<const GameObjectPropertyEditor*>(this)->GetGameObjectHandle());
}

const EditorGameObjectHandle* GameObjectPropertyEditor::GetGameObjectHandle() const
{
    std::shared_ptr<EditorUniversalHandle> shared_obj = m_GameObjectHandle.lock();
    if (shared_obj == nullptr)
        return nullptr;

    return shared_obj->GetGameObjectHandle();
}

void GameObjectPropertyEditor::OnComponentDeleted(size_t index_in_game_object)
{
    IEditableGameObject& game_object = GetGameObjectHandle()->GetGameObject();
    game_object.RemoveComponent(index_in_game_object);

    m_AreComponentEditorsValid = false;
}
