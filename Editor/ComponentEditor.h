#pragma once
#include "../FlatEngine/ComponentDefinition.h"
#include "../FlatEngine/imgui/imgui.h"

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
    void RenderField(const ComponentFieldDefinition* field);
    float CalculateHeight() const;

private:
    ObjectComponent& m_Component;
    ComponentDefinition& m_ComponentDefinition;
    size_t m_IndexInGameObject;

    IComponentActionObserver* m_Observer;
};

