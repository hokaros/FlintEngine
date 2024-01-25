#pragma once
#include "../FlatEngine/ComponentDefinition.h"
#include "../FlatEngine/imgui/imgui.h"
#include "FieldEditor.h"

class IComponentActionObserver
{
public:
    virtual void OnComponentDeleted(size_t index_in_game_object) = 0;

    virtual ~IComponentActionObserver() = default;
};

class ComponentEditor
{
public:
    ComponentEditor(ObjectComponent& component, size_t index_in_game_object);
    void Render();

    void RegisterActionObserver(IComponentActionObserver* observer);

private:
    void CreateFieldEditors();
    void RenderField(FieldEditor& field);
    std::unique_ptr<FieldEditor> CreateFieldEditor(const ComponentFieldDefinition& field);
    float CalculateHeight() const;

private:
    ObjectComponent& m_Component;
    ComponentDefinition& m_ComponentDefinition;
    size_t m_IndexInGameObject;

    std::vector<std::unique_ptr<FieldEditor>> m_FieldEditors;

    IComponentActionObserver* m_Observer;
};

