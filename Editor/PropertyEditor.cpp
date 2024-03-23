#include "PropertyEditor.h"

void PropertyEditor::Init(SelectedObjectManager& selected_game_object_manager)
{
    selected_game_object_manager.SubscribeSelection(*this);
}

void PropertyEditor::Render()
{
    ImGui::Begin("Property Editor");

    EditorGameObjectHandle* game_object_handle = GetGameObjectHandle();
    if (game_object_handle != nullptr && game_object_handle->GetGameObject() != nullptr)
    {
        RenderGameObjectEditor(*game_object_handle->GetGameObject());
    }
    else
    {
        ImGui::Text("No GameObject selected");
    }

    ImGui::End();
}

void PropertyEditor::OnObjectSelected(std::weak_ptr<EditorUniversalHandle> object)
{
    m_GameObjectHandle = object;

    std::shared_ptr<EditorUniversalHandle> shared_obj = object.lock();
    if (shared_obj == nullptr)
        return;

    std::shared_ptr<EditorGameObjectHandle> go_handle = shared_obj->GetGameObjectHandle();
    if (go_handle == nullptr)
        return;

    GameObject& go = go_handle->GetGameObject()->GetResult();
    InitValuesFromGameObject(go);
    LoadComponents(*go_handle->GetGameObject());

    LoadAddableComponents();
}

void PropertyEditor::LoadAddableComponents()
{
    m_AddableComponentNames.clear();
    m_AddableComponents.clear();

    for (const ComponentDefinition* component : ComponentDefinitionManager::GetInstance().GetAllComponents())
    {
        m_AddableComponentNames.push_back(component->GetFileName().c_str());
        m_AddableComponents.push_back(component);
    }
}

void PropertyEditor::RenderGameObjectEditor(IEditableGameObject& game_object)
{
    ImGui::InputText("Name", m_GameObjectName, s_NameMaxSize);

    ImGui::DragFloat2("Position", m_GameObjectPosition, s_SlidersSpeed);
    ImGui::DragFloat2("Size", m_GameObjectSize, s_SlidersSpeed);

    RenderComponentEditors();
    RenderComponentAddSection();

    ApplyValuesToGameObject(game_object);

    if (ImGui::Button("Save"))
    {
        GetGameObjectHandle()->Save();
    }
}

void PropertyEditor::RenderComponentEditors()
{
    if (!m_AreComponentEditorsValid)
    {
        LoadComponents(*GetGameObjectHandle()->GetGameObject()); // TODO: let's pass GameObjectHandle as a parameter
    }

    for (std::unique_ptr<ComponentEditor>& comp_editor : m_ComponentEditors)
    {
        ImGui::Separator();

        comp_editor->Render();
    }
    ImGui::Separator();
}

void PropertyEditor::RenderComponentAddSection()
{
    static int s_ItemCurrent = 0;
    ImGui::Combo("New component", &s_ItemCurrent, m_AddableComponentNames.data(), m_AddableComponentNames.size());
    ImGui::SameLine();
    if (ImGui::Button("+"))
    {
        AddComponent(m_AddableComponents[s_ItemCurrent]);
    }
}

void PropertyEditor::LoadComponents(IEditableGameObject& game_object)
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

void PropertyEditor::AddComponent(const ComponentDefinition* component)
{
    IEditableGameObject& game_object = *GetGameObjectHandle()->GetGameObject();
    game_object.AddComponent(component->GetConstructor()());

    m_AreComponentEditorsValid = false;
}

void PropertyEditor::InitValuesFromGameObject(const GameObject& game_object)
{
    m_GameObjectPosition[0] = game_object.GetPosition().x;
    m_GameObjectPosition[1] = game_object.GetPosition().y;

    m_GameObjectSize[0] = game_object.GetSize().x;
    m_GameObjectSize[1] = game_object.GetSize().y;

    strcpy_s(m_GameObjectName, game_object.GetName().c_str());
}

void PropertyEditor::ApplyValuesToGameObject(IEditableGameObject& game_object)
{
    game_object.SetPosition(Vector(m_GameObjectPosition));

    game_object.SetSize(Vector(m_GameObjectSize));

    std::string name_str = std::string(m_GameObjectName);
    game_object.SetName(name_str);
}

EditorGameObjectHandle* PropertyEditor::GetGameObjectHandle()
{
    std::shared_ptr<EditorUniversalHandle> shared_obj = m_GameObjectHandle.lock();
    if (shared_obj == nullptr)
        return nullptr;

    std::shared_ptr<EditorGameObjectHandle> go_handle = shared_obj->GetGameObjectHandle();
    return go_handle.get();
}

void PropertyEditor::OnComponentDeleted(size_t index_in_game_object)
{
    IEditableGameObject& game_object = *GetGameObjectHandle()->GetGameObject();
    game_object.RemoveComponent(index_in_game_object);

    m_AreComponentEditorsValid = false;
}
