#pragma once
#include <ComponentDefinition.h>
#include <imgui/imgui.h>
#include <AssetHandles/EditorGameObjectHandle.h>
#include "FieldEditor.h"

class IComponentActionObserver
{
public:
    virtual void OnComponentDeleted(size_t index_in_game_object) = 0; // This doesn't mean that the component has been deleted but only that the "Delete" button has been clicked

    virtual ~IComponentActionObserver() = default;
};

class ComponentEditor
{
public:
    ComponentEditor(EditorGameObjectHandle& game_object, ObjectComponent& component, size_t index_in_game_object);
    void Render();

    void RegisterActionObserver(IComponentActionObserver* observer);

private:
    void CreateFieldEditors();
    void RenderField(FieldEditor& field);
    std::unique_ptr<FieldEditor> CreateFieldEditor(const ComponentFieldDefinition& field);
    float CalculateHeight() const;

private:
    EditorGameObjectHandle& m_GameObject;
    ObjectComponent& m_Component;
    ComponentDefinition& m_ComponentDefinition;
    size_t m_IndexInGameObject;

    std::vector<std::unique_ptr<FieldEditor>> m_FieldEditors;

    IComponentActionObserver* m_Observer;
};

